#include <iostream>
#include <fstream>
#include <cstdio>

#include "Patch_download.h"

static size_t write_data(char *buffer, size_t size, size_t nmemb, void *file)
{
	((std::ofstream *)file)->write(buffer, size*nmemb);
	return size*nmemb;
}

static size_t write_data2(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
	return written;
}

void Download_patch_experts()
{
	CURLcode success = curl_global_init(CURL_GLOBAL_ALL);
	std::cout << success << "\n";
	if (success) {
		std::cout << "Failed to initialize cURL.\n";

		// is this necessary?
		curl_global_cleanup();
		return;
	}

	CURL *handle = curl_easy_init();
	if (handle) {
		char errorbuf[CURL_ERROR_SIZE];

		char url[] = "http://www.cs.cmu.edu/~213/assignments.html";
		//char url[] = "https://www.dropbox.com/s/r9n85uwlp604bv5/hello.txt?dl=1";

		// relative path for cert?
		char cert[] = "D:\\Yao Chong\\Documents\\School\\CMU\\Research\\Multi-modal\\CE-CLM\\lib\\3rdParty\\curl\\cacert.pem";
		std::cout << "Downloading from " << url << "\n";

		curl_easy_setopt(handle, CURLOPT_URL, url);
		curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, errorbuf);
		curl_easy_setopt(handle, CURLOPT_CAINFO, cert);
		curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);

		// if successful, save data to patch_experts folder
		// (use CURLOPT_WRITEFUNCTION and CURLOPT_WRITEDATA)?
		// binary?
		std::ofstream output_file("output", std::ios::binary | std::ios::out);
		if (output_file) {
			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, &output_file);
		}

		success = curl_easy_perform(handle);
		if (success != CURLE_OK) {
			std::cout << success << "\n";
			std::cout << errorbuf << "\n";
		}

		output_file.close();

		curl_easy_cleanup(handle);
	}

	// is this necessary?
	curl_global_cleanup();
}