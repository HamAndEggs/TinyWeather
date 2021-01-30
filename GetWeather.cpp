/*
   Copyright (C) 2017, Richard e Collins.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <stdio.h>
#include <curl/curl.h>

#include "GetWeather.h"

OpenWeather::OpenWeather(const std::string& pAPIKey)
{
	curl_global_init(CURL_GLOBAL_DEFAULT);
}

OpenWeather::~OpenWeather()
{
	curl_global_cleanup();
}

void OpenWeather::Get(double pLatitude,double pLongitude,std::function<void()> pReturnFunction)
{ 
	CURL *curl = curl_easy_init();
	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, "https://example.com/");

		// Perform the request, res will get the return code
		CURLcode res = curl_easy_perform(curl);

		// Check for errors
		if(res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));
		}

		/* always cleanup */ 
		curl_easy_cleanup(curl);
	}
}