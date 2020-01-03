// Copyright 2004-2010 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ========================================================================
//
// Constants for dealing with machines.
//
// Manifests are requested over HTTPS. All other network communication goes
// over HTTP.

#ifndef OMAHA_BASE_CONST_ADDRESSES_H_
#define OMAHA_BASE_CONST_ADDRESSES_H_

#include "omaha/base/constants.h"

namespace omaha {

// Static string that gives the main Google website address
// TODO(omaha): Rename this as a connection-check URL. Name should be in caps
// and not include "Google".
#define kGoogleHttpServer L"www." COMPANY_DOMAIN

// Static string used as an identity for the "Omaha" Google domain.
const wchar_t kOmahaPluginServerDomain[] = L"tools." COMPANY_DOMAIN;


// HTTP protocol prefix
#define kProtoSuffix              L"://"
#define kHttpProtoScheme          L"http"
#define kHttpsProtoScheme         L"https"
#define kHttpProto                kHttpProtoScheme kProtoSuffix
#define kHttpsProto               kHttpsProtoScheme kProtoSuffix

// The urls below fall back to http for transport failover purposes. In a small
// number of cases, http requests could succeed even though https requests
// have failed. Since the update checks and pings are critical for update
// functionality, these two types of requests may use unencrypted connections if
// https failed.
//
// Update checks.
// The channel for update checks is secured by using CUP to sign the messages.
// It does not depend solely on https security in any case.
const wchar_t kUrlUpdateCheck[] =
    L"https://update." COMPANY_DOMAIN_BASE L"apis.com/service/update2";

// Pings.
const wchar_t kUrlPing[] =
  L"https://update." COMPANY_DOMAIN_BASE L"apis.com/service/update2";

// The urls below never fall back to http.
//
// Crash reports.
const wchar_t kUrlCrashReport[] =
    L"https://clients2." COMPANY_DOMAIN L"/cr/report";

// More information url.
// Must allow query parameters to be appended to it.
const wchar_t kUrlMoreInfo[] =
    L"https://www." COMPANY_DOMAIN L"/support/installer/?";

// Code Red check url.
const wchar_t kUrlCodeRedCheck[] =
    L"https://clients2." COMPANY_DOMAIN L"/service/check2?crx3=true";

// Usage stats url.
const wchar_t kUrlUsageStatsReport[] =
    L"https://clients5." COMPANY_DOMAIN L"/tbproxy/usagestats";

#if defined(HAS_DEVICE_MANAGEMENT)

// Device Management API url.
const wchar_t kUrlDeviceManagement[] =
    L"https://m." COMPANY_DOMAIN L"/devicemanagement/data/api";

#endif  // defined(HAS_DEVICE_MANAGEMENT)

}  // namespace omaha

#endif  // OMAHA_BASE_CONST_ADDRESSES_H_
