class ram_class {
private:
    int i;
public:
    constexpr ram_class(void): i(0) {};
    int foo() const {
        return 1;
    }
};