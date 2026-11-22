#pragma once

// Runs a blocking lookup (for example getaddrinfo) on a helper thread and waits for it for a limited time.
// A DNS lookup has no timeout of its own, and on some container hosts it can hang for minutes, which used to
// freeze the heartbeat and every player join ("Connecting to server..." forever). Only the standard library is
// used here so this can be tested on its own.

#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <system_error>
#include <thread>

namespace BoundedResolve {

enum class Result {
    Finished, // Out holds the lookup's answer (may be empty if the lookup itself failed)
    TimedOut, // the lookup is still running; it keeps going in the background and calls OnLate when it ends
    NoThread, // the host refused to start a helper thread, nothing was run
};

// Lookup must not touch anything that can disappear (capture by value). OnLate (optional) is called from the
// helper thread with the answer if, and only if, the lookup finishes after we already gave up waiting.
inline Result Run(std::function<std::string()> Lookup, std::chrono::milliseconds Limit, std::string& Out,
    std::function<void(const std::string&)> OnLate = {}) {
    struct State {
        std::mutex Mutex;
        std::condition_variable Cv;
        bool Done = false;
        bool Abandoned = false;
        std::string Answer;
    };
    auto S = std::make_shared<State>();

    try {
        std::thread([S, Lookup = std::move(Lookup), OnLate]() {
            std::string Answer;
            try {
                Answer = Lookup();
            } catch (...) {
            }
            bool Late = false;
            {
                std::lock_guard Lock(S->Mutex);
                S->Answer = Answer;
                S->Done = true;
                Late = S->Abandoned;
            }
            S->Cv.notify_all();
            if (Late && OnLate) {
                try {
                    OnLate(Answer);
                } catch (...) {
                }
            }
        }).detach();
    } catch (const std::system_error&) {
        // The host will not give us another thread (container thread/PID cap). The caller decides what to do.
        return Result::NoThread;
    }

    std::unique_lock Lock(S->Mutex);
    if (S->Cv.wait_for(Lock, Limit, [&] { return S->Done; })) {
        Out = S->Answer;
        return Result::Finished;
    }
    S->Abandoned = true;
    return Result::TimedOut;
}

} // namespace BoundedResolve
