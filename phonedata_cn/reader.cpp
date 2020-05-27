﻿#include "phonedata.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

std::string OUT_GEOCODE_ZH = "../resources/geocoding/zh/86.txt";

std::string OUT_CARRIER_EN = "../resources/carrier/en/86.txt";
std::string OUT_CARRIER_ZH = "../resources/carrier/zh/86.txt";
std::string OUT_CARRIER_ZH_HANT = "../resources/carrier/zh_Hant/86.txt";

bool cmp(PhoneInfo a, PhoneInfo b)
{
	return a.phone < b.phone;
}

int main()
{
	PhoneData data;
	std::vector<PhoneInfo> infoVec = data.loadAll();
	
	if (!infoVec.empty()) {
		std::ofstream geoCode_zh(OUT_GEOCODE_ZH, std::ios::out);
		std::ofstream carrier_en(OUT_CARRIER_EN, std::ios::out);
		std::ofstream carrier_zh(OUT_CARRIER_ZH, std::ios::out);
		std::ofstream carrier_zh_hant(OUT_CARRIER_ZH_HANT, std::ios::out);

		std::sort(infoVec.begin(), infoVec.end(), cmp);

		for (int i = 0; i < infoVec.size(); i++) {
			PhoneInfo info = infoVec[i];
			if (i + 9 < infoVec.size() && info.phone % 10 == 0 && infoVec[i+9].areaCode == info.areaCode) {
				i += 9;
				geoCode_zh << "86" << (info.phone / 10) << "|";
			} else {
				geoCode_zh << "86" << info.phone << "|";
			}
			// 直辖市
			if (info.province == info.city) {
				geoCode_zh << info.city << "市" << std::endl;
			} else {
				// 少数民族自治区
				if (info.province == "内蒙古" || info.province == "西藏" || 
					info.province == "广西" || info.province == "宁夏" || info.province == "新疆") {
					geoCode_zh << info.province << ", " << info.city << "市" << std::endl;
				} else {
					geoCode_zh << info.province << "省" << info.city << "市" << std::endl;
				}
			}
		}
		for (int i = 0; i < infoVec.size(); i++) {
			PhoneInfo info = infoVec[i];
			int comPos = 9;
			while (i + comPos < infoVec.size() && (info.phone % (comPos + 1) == 0) && (info.phone / (comPos + 1) == infoVec[i+comPos].phone / (comPos + 1))
					&& info.type == infoVec[i+comPos].type)
			{
				comPos *= 10;
				comPos += 9;
			}
			comPos /= 10;
			i += comPos;
			carrier_en << "86" << info.phone / (comPos+1) << "|" << getPhoneType(info.type, en) << std::endl;
			carrier_zh << "86" << info.phone / (comPos+1) << "|" << getPhoneType(info.type, zh_Hans) << std::endl;
			carrier_zh_hant << "86" << info.phone / (comPos+1) << "|" << getPhoneType(info.type, zh_Hant) << std::endl;
		}
	}
	return 0;
}