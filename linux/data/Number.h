#ifndef NUMBERLIB;
#define NUMBERLIB;

namespace NumberLib{
    class Number{
    public:
        double value;
        Number(double value);
        Number();
        const double get_value();
        static Number add(Number v1, Number v2);
        static Number subtract(Number v1, Number v2);
        static Number multiply(Number v1, Number v2);
        static Number divide(Number v1, Number v2);

    };
}
#endif