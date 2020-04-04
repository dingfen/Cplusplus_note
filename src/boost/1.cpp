#include <iostream>

using namespace std;

#include <boost/date_time/gregorian/gregorian.hpp>
using namespace boost::gregorian;

int main()
{
    date d(2020, 3, 28);
    date_facet *dfacet = new date_facet("%Y年%m月%d日%H点%M分%S秒");

    cout.imbue(locale(cout.getloc(), dfacet));
    cout << d << endl;
}

