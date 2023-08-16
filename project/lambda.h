#ifndef LAMBDA_H
#define LAMBDA_H

/*https://stackoverflow.com/questions/7852101/c-lambda-with-captures-as-a-function-pointer*/

/**
 * @brief A structure that provides functions and methods for handling lambda expressions.
 */
struct Lambda {
    /**
     * @brief Executes a lambda expression provided through a function pointer.
     * 
     * @tparam Tret The return type of the lambda expression.
     * @tparam T The type of the lambda expression.
     * @tparam Arg The type of argument data being passed to the lambda expression.
     * @param data A pointer to the argument data being passed to the lambda expression.
     * @return The return value of the lambda expression.
     */
    template<typename Tret, typename T, typename Arg>
    static Tret lambda_ptr_exec(Arg* data) {
        return (Tret) (*(T*)fn<T>())(data);
    }

    /**
     * @brief Returns a function pointer to the given lambda expression.
     * 
     * @tparam Tret The return type of the lambda expression.
     * @tparam Tfp The type of function pointer.
     * @tparam T The type of the lambda expression.
     * @param t A reference to the lambda expression.
     * @return A function pointer to the lambda expression.
     */
    template<typename Tret = void, typename Tfp = Tret(*)(void*), typename T>
    static Tfp ptr(T& t) {
        fn<T>(&t);
        return (Tfp) lambda_ptr_exec<Tret, T>;
    }

    /**
     * @brief Returns or updates a pointer to the lambda expression.
     * 
     * @tparam T The type of the lambda expression.
     * @param new_fn An optional pointer to a new lambda expression.
     * @return A pointer to the current lambda expression.
     */
    template<typename T>
    static T* fn(T* new_fn = nullptr) {
        static T* fn;
        if (new_fn != nullptr)
            fn = new_fn;
        return fn;
    }
};

#endif //LAMBDA_H