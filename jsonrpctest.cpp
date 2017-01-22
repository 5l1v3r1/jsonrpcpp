/***
    This file is part of jsonrpc
    Copyright (C) 2017 Johannes Pohl
    
    This software may be modified and distributed under the terms
    of the MIT license.  See the LICENSE file for details.
***/

#include "jsonrpc.h"


void test(const std::string& json_str)
{
	try
	{
		std::cout << "Test: " << json_str << "\n";
		jsonrpc::Parser::parse(json_str);
	}
	catch(const jsonrpc::RequestException& e)
	{
		std::cerr << "RequestException: " << e.what() << "\n";
	}
	catch(const jsonrpc::RpcException& e)
	{
		std::cerr << "RpcException: " << e.what() << "\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	std::cout << "\n";
}


//example taken from: http://www.jsonrpc.org/specification#examples
int main(int argc, char* argv[])
{
	//rpc call with positional parameters:
	test(R"({"jsonrpc": "2.0", "method": "subtract", "params": [42, 23], "id": 1})");
	test(R"({"jsonrpc": "2.0", "method": "subtract", "params": [23, 42], "id": 2})");

	//rpc call with named parameters:
	test(R"({"jsonrpc": "2.0", "method": "subtract", "params": {"subtrahend": 23, "minuend": 42}, "id": 3})");
	test(R"({"jsonrpc": "2.0", "method": "subtract", "params": {"minuend": 42, "subtrahend": 23}, "id": 4})");

	//a Notification:
	test(R"({"jsonrpc": "2.0", "method": "update", "params": [1,2,3,4,5]})");
	test(R"({"jsonrpc": "2.0", "method": "foobar"})");

	//rpc call of non-existent method:
	test(R"({"jsonrpc": "2.0", "method": "foobar", "id": "1"})");

	//rpc call with invalid JSON:
	test(R"({"jsonrpc": "2.0", "method": "foobar, "params": "bar", "baz])");

	//rpc call with invalid Request object:
	test(R"({"jsonrpc": "2.0", "method": 1, "params": "bar"})");

	//rpc call Batch, invalid JSON:
	test(R"(
	[
		{"jsonrpc": "2.0", "method": "sum", "params": [1,2,4], "id": "1"},
		{"jsonrpc": "2.0", "method"
	])");

	//rpc call with an empty Array:
	test(R"([])");

	//rpc call with an invalid Batch (but not empty):
	test(R"([1])");

	//rpc call with invalid Batch:
	test(R"([1,2,3])");

	//rpc call Batch:
	test(R"(
	[
		{"jsonrpc": "2.0", "method": "sum", "params": [1,2,4], "id": "1"},
		{"jsonrpc": "2.0", "method": "notify_hello", "params": [7]},
		{"jsonrpc": "2.0", "method": "subtract", "params": [42,23], "id": "2"},
		{"foo": "boo"},
		{"jsonrpc": "2.0", "method": "foo.get", "params": {"name": "myself"}, "id": "5"},
		{"jsonrpc": "2.0", "method": "get_data", "id": "9"} 
	])");

	//rpc call Batch (all notifications):
	test(R"(
	[
		{"jsonrpc": "2.0", "method": "notify_sum", "params": [1,2,4]},
		{"jsonrpc": "2.0", "method": "notify_hello", "params": [7]}
	])");
}


