function update(delta)
    log("hello")
    print(delta)
    delta = 10
    return "!ahh"
end

first = {
    x = 11,
    nested = {
        name = "dave",
        another_nested = {
        }
    },
    another = {
        alive = false;
    },
}
first2 = {
    nested = {
        another_nested = {
        }
    },
}

indexedTable = {
    "banana",
    2,
    false,
    {
        "dave",
        3.33,
        {
            "bla",
            234
        }
    }
}