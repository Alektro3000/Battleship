
#include <iostream>

#include <boost/any.hpp>

int main()
{
    boost::any in, fl;
    in = 1;
    fl = 1.123f;
    std::cout << boost::any_cast<float>(fl)<<' '<< boost::any_cast<int>(in) <<std::endl;
}
