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

#ifndef GET_WEATHER_H
#define GET_WEATHER_H

#include <string>
#include <vector>
#include <functional>

struct TheWeather
{
	double mLatitude;		//!< Geographical coordinates of the location (latitude)
	double mLongitude;		//!< Geographical coordinates of the location (longitude)
	std::string mTimeZone;	//!< timezone Timezone name for the requested location
	uint32_t mTimezoneOffset;	//!< timezone_offset Shift in seconds from UTC
	struct
	{
		uint64_t mTime;				//!< Current time, Unix, UTC
		uint64_t mSunrise;			//!< Sunrise time, Unix, UTC
		uint64_t mSunset;			//!< Sunset time, Unix, UTC
		float mTemperature;			//!< Temperature. Units - default: kelvin, metric: Celsius, imperial: Fahrenheit. How to change units used
		float mFeelsLike;			//!< This temperature parameter accounts for the human perception of weather. Units – default: kelvin, metric: Celsius, imperial: Fahrenheit.
		uint32_t mPressure;			//!< Atmospheric pressure on the sea level, hPa
		uint32_t mHumidity;			//!< Humidity, %
		float mDewPoint;			//!< Atmospheric temperature (varying according to pressure and humidity) below which water droplets begin to condense and dew can form. Units – default: kelvin, metric: Celsius, imperial: Fahrenheit.
		uint32_t mClouds;			//!< Cloudiness, %
		uint32_t mUVIndex;			//!< Current UV index
		uint32_t mVisibility;		//!< Average visibility, metres
		float mWindSpeed;			//!< Wind speed. Wind speed. Units – default: metre/sec, metric: metre/sec, imperial: miles/hour. How to change units used
		float mWindGusts;			//!< defaults to 0 if not found. (where available) Wind gust. Units – default: metre/sec, metric: metre/sec, imperial: miles/hour. How to change units used
		uint32_t mWindDirection;	//!< Wind direction, degrees (meteorological)
		uint32_t mRainVolume;		//!< (where available) Rain volume for last hour, mm
		uint32_t mSnowVolume;		//!< (where available) Snow volume for last hour, mm
		struct
		{
			uint32_t mID;				//!< Weather condition id
			std::string mTitle;			//!< Group of weather parameters (Rain, Snow, Extreme etc.)
			std::string mDescription;	//!< Weather condition within the group (full list of weather conditions). Get the output in your language
			std::string mIcon;			//!< Weather icon id. How to get icons
		}mWeather;
	}mCurrent; //<! Current weather data API response

	std::vector<struct
	{
		uint64_t mTime;
		uint32_t mPrecipitation;
	}>mMinutely;

	//!< minutely Minute forecast weather data API response
	//!< minutely.dt Time of the forecasted data, unix, UTC
	//!< minutely.precipitation Precipitation volume, mm
	//!< hourly Hourly forecast weather data API response
	//!< hourly.dt Time of the forecasted data, Unix, UTC
	//!< hourly.temp Temperature. Units – default: kelvin, metric: Celsius, imperial: Fahrenheit. How to change units used
	//!< hourly.feels_like Temperature. This accounts for the human perception of weather. Units – default: kelvin, metric: Celsius, imperial: Fahrenheit.
	//!< hourly.pressure Atmospheric pressure on the sea level, hPa
	//!< hourly.humidity Humidity, %
	//!< hourly.dew_point Atmospheric temperature (varying according to pressure and humidity) below which water droplets begin to condense and dew can form. Units – default: kelvin, metric: Celsius, imperial: Fahrenheit.
	//!< hourly.uvi UV index
	//!< hourly.clouds Cloudiness, %
	//!< hourly.visibility Average visibility, metres
	//!< hourly.wind_speed Wind speed. Units – default: metre/sec, metric: metre/sec, imperial: miles/hour.How to change units used
	//!< hourly.wind_gust (where available) Wind gust. Units – default: metre/sec, metric: metre/sec, imperial: miles/hour. How to change units used
	//!< chourly.wind_deg Wind direction, degrees (meteorological)
	//!< hourly.pop Probability of precipitation
	//!< hourly.rain
	//!< hourly.rain.1h (where available) Rain volume for last hour, mm
	//!< hourly.snow
	//!< hourly.snow.1h (where available) Snow volume for last hour, mm
	//!< hourly.weather
	//!< hourly.weather.id Weather condition id
	//!< hourly.weather.main Group of weather parameters (Rain, Snow, Extreme etc.)
	//!< hourly.weather.description Weather condition within the group (full list of weather conditions). Get the output in your language
	//!< hourly.weather.icon Weather icon id. How to get icons
	//!< daily Daily forecast weather data API response
	//!< daily.dt Time of the forecasted data, Unix, UTC
	//!< daily.sunrise Sunrise time, Unix, UTC
	//!< daily.sunset Sunset time, Unix, UTC
	//!< daily.temp Units – default: kelvin, metric: Celsius, imperial: Fahrenheit. How to change units used
	//!< daily.temp.morn Morning temperature.
	//!< daily.temp.day Day temperature.
	//!< daily.temp.eve Evening temperature.
	//!< daily.temp.night Night temperature.
	//!< daily.temp.min Min daily temperature.
	//!< daily.temp.max Max daily temperature.
	//!< daily.feels_like This accounts for the human perception of weather. Units – default: kelvin, metric: Celsius, imperial: Fahrenheit. How to change units used
	//!< daily.feels_like.morn Morning temperature.
	//!< daily.feels_like.day Day temperature.
	//!< daily.feels_like.eve Evening temperature.
	//!< daily.feels_like.night Night temperature.
	//!< daily.pressure Atmospheric pressure on the sea level, hPa
	//!< daily.humidity Humidity, %
	//!< daily.dew_point Atmospheric temperature (varying according to pressure and humidity) below which water droplets begin to condense and dew can form. Units – default: kelvin, metric: Celsius, imperial: Fahrenheit.
	//!< daily.wind_speed Wind speed. Units – default: metre/sec, metric: metre/sec, imperial: miles/hour. How to change units used
	//!< daily.wind_gust (where available) Wind gust. Units – default: metre/sec, metric: metre/sec, imperial: miles/hour. How to change units used
	//!< daily.wind_deg Wind direction, degrees (meteorological)
	//!< daily.clouds Cloudiness, %
	//!< daily.uvi The maximum value of UV index for the day
	//!< daily.pop Probability of precipitation
	//!< daily.rain (where available) Precipitation volume, mm
	//!< daily.snow (where available) Snow volume, mm
	//!< daily.weather
	//!< daily.weather.id Weather condition id
	//!< daily.weather.main Group of weather parameters (Rain, Snow, Extreme etc.)
	//!< daily.weather.description Weather condition within the group (full list of weather conditions). Get the output in your language
	//!< daily.weather.icon Weather icon id. How to get icons
	//!< alerts National weather alerts data from major national weather warning systems
	//!< alerts.sender_name Name of the alert source. Please read here the full list of alert sources
	//!< alerts.event Alert event name
	//!< alerts.start Date and time of the start of the alert, Unix, UTC
	//!< alerts.end Date and time of the end of the alert, Unix, UTC
	//!< alerts.description Description of the alert
};

/**
 * @brief Builds a tree of data that you can read that represents the weather for your area.
 * uses OpenWeather one-call-api https://openweathermap.org/api/one-call-api
 * You will need to make a free account and get an API key
 */
class OpenWeather
{
public:
	OpenWeather(const std::string& pAPIKey);
	~OpenWeather();

	void Get(double pLatitude,double pLongitude,std::function<void(const TheWeather& pWeather)> pReturnFunction);

private:

	const std::string mAPIKey;

};
	
#endif //GET_WEATHER_H
