/*
 * Copyright (C) 2017  Andres Rodriguez
 *
 * This file is part of gputool.
 *
 * gputool is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * gputool is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gputool.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <exception>
#include <memory>

/**
 * This file contains some helpers that may be useful between different
 * projects
 */

namespace util
{
class TerminationException : public std::exception
{
};

/**
 * Print a message and exit with the specified error code
 */
static void die(const char* format, ...)
{
    va_list vargs;
    va_start(vargs, format);
    vfprintf(stderr, format, vargs);
    va_end(vargs);
    throw TerminationException();
}

class FailureException : public std::exception
{
};

/**
 * Print a message and exit with the specified error code
 */
#define failOn(condition, format, ...)                         \
    do {                                                       \
        if (condition) {                                       \
            fprintf(stderr, "Check failed: %s\n", #condition); \
            util::__failOn(format, ##__VA_ARGS__);             \
        }                                                      \
    } while (0)

static inline void __failOn(const char* format, ...)
{
    va_list vargs;
    va_start(vargs, format);
    vfprintf(stderr, format, vargs);
    va_end(vargs);
    throw FailureException();
}

/**
 * Because the C pre-processor is awesome
 */
#define CONCATENATE_IMPL(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_IMPL(s1, s2)

/**
 * Simple make_unique sample implementation from cppreference
 */
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

/**
 * Get the size of an array
 */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
//-----------------------------------------------------------------------------

/**
 * The following section contains Andrei Alexandrescu's scope guard
 * implementation as described in his CppCon talk “Declarative Control Flow"
 *
 * The talk can be found here:
 * https://www.youtube.com/watch?v=WjTrfoiB0MQ
 */

/**
 * Allow the creation of an anonymous variable
 */
#ifdef __COUNTER__
#define ANONYMOUS_VARIABLE(str) CONCATENATE(str, __COUNTER__)
#else
#define ANONYMOUS_VARIABLE(str) CONCATENATE(str, __LINE__)
#endif

/**
 * Execute a lambda expression when exiting the scope
 */
#define SCOPE_EXIT                              \
    auto ANONYMOUS_VARIABLE(SCOPE_EXIT_STATE) = \
        ::util::ScopeGuardOnExit() + [&]() noexcept

/**
 * Execute a lambda expression only if we are exiting the
 * scope due to an exception
 */
#define SCOPE_FAIL                              \
    auto ANONYMOUS_VARIABLE(SCOPE_FAIL_STATE) = \
        ::util::ScopeGuardOnFail() + [&]() noexcept

/**
 * Execute a lambda expression only if we are exiting the
 * scope due without encountering an exception
 */
#define SCOPE_SUCCESS \
    auto ANONYMOUS_VARIABLE(SCOPE_SUCCESS_STATE) = ::util::ScopeGuardOnSuccess() + [&]()

class UncaughtExceptionCounter
{
    int getUncaughtExceptionCount() noexcept;
    int exceptionCount_;

  public:
    UncaughtExceptionCounter() : exceptionCount_(std::uncaught_exceptions()) {}
    bool isNewUncaughtException() noexcept
    {
        return std::uncaught_exceptions() > exceptionCount_;
    }
};

typedef enum ExecuteType {
    ExecuteAlways = 0,  // EXIT
    ExecuteOnFail,      // FAIL
    ExecuteOnSuccess    // SUCCESS
} ExecuteType;

template <typename FunctionType, ExecuteType executeOnException>
class ScopeGuardForNewException
{
  private:
    FunctionType function_;
    UncaughtExceptionCounter ec_;

  public:
    ScopeGuardForNewException(const FunctionType& fn) : function_(fn){};
    ScopeGuardForNewException(FunctionType&& fn) : function_(std::move(fn)){};
    ~ScopeGuardForNewException() noexcept(executeOnException)
    {
        bool shouldExec = false;

        switch (executeOnException) {
            case ExecuteAlways:
                shouldExec = true;
                break;
            case ExecuteOnFail:
                shouldExec = ec_.isNewUncaughtException();
                break;
            case ExecuteOnSuccess:
                shouldExec = !ec_.isNewUncaughtException();
                break;
            default:
                die("Invalid scope ExecuteType\n");
        }

        if (shouldExec)
            function_();
    }
};

enum class ScopeGuardOnExit {};

template <typename FunctionType>
ScopeGuardForNewException<typename std::decay<FunctionType>::type, ExecuteAlways>
operator+(util::ScopeGuardOnExit, FunctionType&& fn)
{
    return ScopeGuardForNewException<typename std::decay<FunctionType>::type,
                                     ExecuteAlways>(std::forward<FunctionType>(fn));
}

enum class ScopeGuardOnFail {};

template <typename FunctionType>
ScopeGuardForNewException<typename std::decay<FunctionType>::type, ExecuteOnFail>
operator+(util::ScopeGuardOnFail, FunctionType&& fn)
{
    return ScopeGuardForNewException<typename std::decay<FunctionType>::type,
                                     ExecuteOnFail>(std::forward<FunctionType>(fn));
}

enum class ScopeGuardOnSuccess {};

template <typename FunctionType>
ScopeGuardForNewException<typename std::decay<FunctionType>::type, ExecuteOnSuccess>
operator+(util::ScopeGuardOnSuccess, FunctionType&& fn)
{
    return ScopeGuardForNewException<typename std::decay<FunctionType>::type,
                                     ExecuteOnSuccess>(std::forward<FunctionType>(fn));
}

//-----------------------------------------------------------------------------
};  // namespace util
