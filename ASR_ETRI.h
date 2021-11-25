#pragma once

#include "WAV.h"

#include "curl.h"
#include <iostream>
#include <string>
#include <memory.h>
#include <fstream>
#include <math.h>

#include "json.hpp"

#include <thread>
#include <atomic>

#define LENFRAME  800

using json = nlohmann::json;
using string = std::string;

const string  base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";


static string tmp_response;
inline size_t writeDataFunc(void* ptr, size_t size, size_t nmemb, string stream);
inline string base64_encode(unsigned char const*, unsigned int len);

class ASR_ETRI {

private:
  char* openApiURL;
  char* audioFilePath;

  string accessKey;
  string languageCode;

  std::thread* thread_asr=nullptr;
  std::atomic<bool> bool_thread=false;

  
public:

  inline ASR_ETRI(string accessKey, string languageCode);
  inline ~ASR_ETRI();
  inline string Request(string filePath);

};


ASR_ETRI::ASR_ETRI(string accessKey_,string languageCode_) {
  openApiURL = (char*)"https://aiopen.etri.re.kr:8443/WiseASR/Recognition";
  accessKey = accessKey_;
  languageCode = languageCode_;
}

ASR_ETRI::~ASR_ETRI() {}


string ASR_ETRI::Request(string filePath){

  FILE* fp = fopen(filePath.c_str(), "rt");
  unsigned char sbuf[LENFRAME];
  unsigned int nRead = 0;
  unsigned int nWrite = 0;

  unsigned char* audioBytes;
  unsigned char* newAudioBytes;


  if (fp == NULL) {
    std::cout << "Failed to Open File\n" << std::endl;
    exit(-1);
  }

  fseek(fp, 0L, SEEK_END);
  nWrite = ftell(fp)-44;

  fseek(fp, 44, SEEK_SET);

/*
  while (0 < (nRead = fread(sbuf, sizeof(char), LENFRAME, fp))) {
    newAudioBytes = new unsigned char[nWrite + nRead];
    if (0 < nWrite) {
      memcpy(newAudioBytes, audioBytes, nWrite * sizeof(char));
    }
    memcpy(newAudioBytes + nWrite, sbuf, nRead * sizeof(char));
    nWrite = nWrite + nRead;
    audioBytes = new unsigned char[nWrite];
    memcpy(audioBytes, newAudioBytes, nWrite * sizeof(char));
  }
  */
  audioBytes = new unsigned char[nWrite];
  fread(audioBytes,sizeof(unsigned char),nWrite,fp);
  fclose(fp);

  json request;
  json argument;

  argument["language_code"] = languageCode;
  argument["audio"] = base64_encode(audioBytes, nWrite);

  request["access_key"] = accessKey;
  request["argument"] = argument;

  CURL* curl;
  curl_slist* responseHeaders = NULL;
  curl = curl_easy_init();


  string response;
  if (curl == NULL) {
    std::cout << "curl :: NULL" << std::endl;
  }
  else {
    responseHeaders = curl_slist_append(responseHeaders, "Content-Type: application/json; charset=UTF-8");
    string requestJson = request.dump();
    long statusCode=-1;

    curl_easy_setopt(curl, CURLOPT_URL, openApiURL);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, responseHeaders);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestJson.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeDataFunc);
   curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed : %s\n", curl_easy_strerror(res));

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
    curl_easy_cleanup(curl);

   // std::cout << "statusCode : " << statusCode << std::endl;


    // statusCode
    // response
  }

  request.clear();

  delete[] audioBytes;
 // delete[] newAudioBytes;

  // TODO : WriteDataFunc get response properly,
  // But when curl routine is done
  // reponse is empty.
  // So temporally use tmp_response; 
  response = tmp_response;

  if (!response.empty()) {
    json ret = json::parse(response);
    //std::cout << ret.dump();

    if (ret["result"].get<int>() == -1) {
      //printf("ERROR\n");
      return ret["reason"].get<string>();
    }
    return ret["return_object"]["recognized"].get<string>();
  }
  else
    return "";

  //return ret["return_object"]["recognized"].get<string>();
}

size_t writeDataFunc(void* ptr, size_t size, size_t nmemb, string stream) {
  size_t realsize = size * nmemb;
  string temp(static_cast<const char*>(ptr), realsize);
  stream.append(temp);
  stream = temp;
  tmp_response = temp;
  return realsize;
}

string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
  string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for (i = 0; (i < 4); i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while ((i++ < 3))
      ret += '=';
  }
  return ret;

}