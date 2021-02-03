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

#include <iostream>
#include <sstream>

#include <stdio.h>
#include <curl/curl.h>

#include "GetWeather.h"
#include "TinyJson.h"


static int CURLWriter(char *data, size_t size, size_t nmemb,std::string *writerData)
{
	if(writerData == NULL)
		return 0;

	writerData->append(data, size*nmemb);

	return size * nmemb;
}

GetWeather::GetWeather(const std::string& pAPIKey):mAPIKey(pAPIKey)
{
	curl_global_init(CURL_GLOBAL_DEFAULT);
}

GetWeather::~GetWeather()
{
	curl_global_cleanup();
}

void GetWeather::Get(double pLatitude,double pLongitude,std::function<void(const TheWeather& pWeather)> pReturnFunction)
{ 
	std::string jsonData;
	std::stringstream url;
	url << "http://api.openweathermap.org/data/2.5/onecall?";
	url << "lat=" << pLatitude << "&";
	url << "lon=" << pLongitude << "&";
	url << "appid=" << mAPIKey;

	if( DownloadWeatherReport(url.str(),jsonData) )
	{
		// We got it, now we need to build the weather object from the json.
		// I would have used rapid json but that is a lot of files to add to this project.
		// My intention is for someone to beable to drop these two files into their project and continue.
		// And so I will make my own json reader, it's easy but not the best solution.
		tinyjson::JsonProcessor theWeather(jsonData);

	}
}

bool GetWeather::DownloadWeatherReport(const std::string& pURL,std::string& rJson)const
{
	bool result = false;
	CURL *curl = curl_easy_init();
	if(curl)
	{
		char errorBuffer[CURL_ERROR_SIZE];
		errorBuffer[0] = 0;

		const char* funcName = "CURLOPT_ERRORBUFFER";
		if( curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer) == CURLE_OK )
		{
			funcName = "CURLOPT_URL";
			if( curl_easy_setopt(curl, CURLOPT_URL, pURL.c_str()) == CURLE_OK )
			{
				funcName = "CURLOPT_WRITEFUNCTION";
				if( curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CURLWriter) == CURLE_OK )
				{
					funcName = "CURLOPT_WRITEDATA";
					if( curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rJson) == CURLE_OK )
					{
						funcName = "curl_easy_perform";
						if( curl_easy_perform(curl) == CURLE_OK )
						{
							result = true;
						}
					}
				}
			}
		}

		if( result  == false )
		{
			std::cerr << "Lib curl " << funcName << " failed, [" << errorBuffer << "]\n";
		}

		/* always cleanup */ 
		curl_easy_cleanup(curl);
	}

	return result;
}

