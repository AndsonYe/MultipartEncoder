#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "multipart_parser.h"

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

int main(int argc, char* argv[])
{
    auto fileStream = std::make_shared<ostream>();

    // Open stream to output file.
    pplx::task<void> requestTask = fstream::open_ostream(U("results")).then([=](ostream outFile)
    {
        *fileStream = outFile;

        //Use MultipartParser to get the encoded body content and boundary
        MultipartParser parser;
        parser.AddParameter("Filename", "1.jpg");
        parser.AddFile("file", "1.jpg");
        std::string boundary = parser.boundary();
        std::string body = parser.GenBodyContent();
        //std::cout << body << std::endl;

        //Set up http client and request
        http_request req;
        http_client client(U("http://www.filedropper.com/index.php?xml=true"));
        req.set_method(web::http::methods::POST);
        req.set_body(body, "multipart/form-data; boundary=" + boundary);
        return client.request(req);
    })
    .then([=](pplx::task<http_response> response_task)
    {
        http_response response = response_task.get();
        return response.body().read_to_end(fileStream->streambuf());
    })
    .then([=](size_t)
    {
        return fileStream->close();
    });

    // Wait for all the outstanding I/O to complete and handle any exceptions
    try
    {
        requestTask.wait();
        //postTask.wait();
    }
    catch (const std::exception &e)
    {
        printf("Error exception:%s\n", e.what());
    }

    return 0;
}
