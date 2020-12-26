//
// Created by Zhengyi on 2020/11/17.
//

#include "BigInteger.h"

int main() {
    using namespace std;
    auto t = BigInteger("99");
    auto b = BigInteger("-98");
    (t + b).show(std::cout) << '\n';
    (BigInteger("990") - BigInteger("-1")).show(std::cout);
    cout << "\n";
    (BigInteger("102935483") * BigInteger("99")).show(std::cout);
    cout << "\n";
    (BigInteger("102935483") / BigInteger("99")).show(std::cout);
    cout << "\n";
    (BigInteger("99") / BigInteger("99")).show(std::cout);
    cout << "\n";

    (BigInteger("99") ^ BigInteger("99")).show(std::cout);
    cout << "\n";
    return 0;
}
