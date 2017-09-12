#include <iostream>
#include <fstream>
#include <cstdio>

#include <curl/curl.h>

#include "File_download.h"

static size_t write_data(char *buffer, size_t size, size_t nmemb, void *file)
{
	((std::ofstream *)file)->write(buffer, size*nmemb);
	return size*nmemb;
}

// TODO: add argument to program to download certificate for SSL?
// necessary to put this in a class?
void Download_file(const char *url, const char *filename)
{
	// For safety, ensure url and filename are null-terminated?

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

		// char cert[] = "..\\..\\..\\3rdParty\\curl\\cacert.pem";
		std::cout << "Downloading from " << url << "\n";

		curl_easy_setopt(handle, CURLOPT_URL, url);
		curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, errorbuf);
		//curl_easy_setopt(handle, CURLOPT_CAINFO, cert);
		// show progress bar
		curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 0L);

		// if successful, save data to patch_experts folder
		std::ofstream output_file(filename, std::ios::binary | std::ios::out);
		if (output_file.is_open()) {

			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, &output_file);
			success = curl_easy_perform(handle);
			if (success != CURLE_OK) {
				std::cout << success << "\n";
				std::cout << errorbuf << "\n";
			}

			output_file.close();
		}
		curl_easy_cleanup(handle);
	}
	curl_global_cleanup();
}