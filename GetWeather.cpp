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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
   
   Original code base is at https://github.com/HamAndEggs/GetWeather
*/

#include <iostream>
#include <sstream>
#include <assert.h>

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
	assert( pReturnFunction != nullptr );

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
		tinyjson::JsonProcessor json(jsonData);
		const tinyjson::JsonValue weather = json.GetRoot();

		TheWeather weatherData;

		// Lets build up the weather data.
		if( weather.HasValue("current") )
		{
			tinyjson::JsonValue current = weather["current"];
			weatherData.mCurrent.mTime = current.GetUInt64("dt");					//!< Current time, Unix, UTC
			weatherData.mCurrent.mSunrise = current.GetUInt64("sunrise");			//!< Sunrise time, Unix, UTC
			weatherData.mCurrent.mSunset = current.GetUInt64("sunset");				//!< Sunset time, Unix, UTC
			weatherData.mCurrent.mTemperature = current.GetFloat("temp");			//!< Temperature. Units - default: kelvin, metric: Celsius, imperial: Fahrenheit. How to change units used
			weatherData.mCurrent.mFeelsLike = current.GetFloat("feels_like");		//!< This temperature parameter accounts for the human perception of weather. Units – default: kelvin, metric: Celsius, imperial: Fahrenheit.
			weatherData.mCurrent.mPressure = current.GetUInt64("pressure");			//!< Atmospheric pressure on the sea level, hPa
			weatherData.mCurrent.mHumidity = current.GetUInt64("humidity");			//!< Humidity, %
			weatherData.mCurrent.mDewPoint = current.GetFloat("dew_point");			//!< Atmospheric temperature (varying according to pressure and humidity) below which water droplets begin to condense and dew can form. Units – default: kelvin, metric: Celsius, imperial: Fahrenheit.
			weatherData.mCurrent.mClouds = current.GetUInt32("clouds");				//!< Cloudiness, %
			weatherData.mCurrent.mUVIndex = current.GetUInt32("uvi");				//!< Current UV index
			weatherData.mCurrent.mVisibility = current.GetUInt32("visibility");		//!< Average visibility, metres
			weatherData.mCurrent.mWindSpeed = current.GetFloat("wind_speed");		//!< Wind speed. Wind speed. Units – default: metre/sec, metric: metre/sec, imperial: miles/hour. How to change units used
			weatherData.mCurrent.mWindGusts = current.GetFloat("wind_deg");			//!< defaults to 0 if not found. (where available) Wind gust. Units – default: metre/sec, metric: metre/sec, imperial: miles/hour. How to change units used
			weatherData.mCurrent.mWindDirection = current.GetUInt32("wind_deg");	//!< Wind direction, degrees (meteorological)

			if( current.GetArraySize("weather") > 0 )
			{
				tinyjson::JsonValue weather = current["weather"][0];
				weatherData.mCurrent.mWeather.mID = weather.GetUInt32("id");
				weatherData.mCurrent.mWeather.mTitle = weather.GetString("main");
				weatherData.mCurrent.mWeather.mDescription = weather.GetString("description");
				weatherData.mCurrent.mWeather.mIcon = weather.GetString("icon");
			}
		}


		pReturnFunction(weatherData);
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

