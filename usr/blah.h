class rom_class {
private:
    int i;
public:
    constexpr rom_class(void): i(0) {};
    int foo() const {
        return 1;
    }
};