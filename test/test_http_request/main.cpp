
#include <iostream>

#include "utility/utility.h"
#include "asio/frame_work.h"
#include "asio/http_request.h"

using namespace std;
using namespace QTH_NAME_SPACE;

int main(int argc, char* argv[])
{
	CHttpRequest cHttpRequest;
	if (argc < 2) {
		cout << "please input:\n\t./test_http_request url" << endl;
		return 0;
	}
	std::string url(argv[1]);
	cHttpRequest.Debug(url);

	cerr << cHttpRequest.Get(url) <<endl;
	return 0;
}
