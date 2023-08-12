#ifndef LAMBDA_H
#define LAMBDA_H

/*https://stackoverflow.com/questions/7852101/c-lambda-with-captures-as-a-function-pointer*/
struct Lambda {
    template<typename Tret, typename T, typename Arg>
    static Tret lambda_ptr_exec(Arg* data) {
        return (Tret) (*(T*)fn<T>())(data);
    }

    template<typename Tret = void, typename Tfp = Tret(*)(void*), typename T>
    static Tfp ptr(T& t) {
        fn<T>(&t);
        return (Tfp) lambda_ptr_exec<Tret, T>;
    }

    template<typename T>
    static void* fn(void* new_fn = nullptr) {
        static void* fn;
        if (new_fn != nullptr)
            fn = new_fn;
        return fn;
    }
};

#endif //LAMBDA_H