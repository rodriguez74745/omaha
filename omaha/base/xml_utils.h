// Copyright 2005-2009 Google Inc.
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
// xml_utils.h
//
// Utilities for working with XML files via MSXML.

#ifndef OMAHA_BASE_XML_UTILS_H_
#define OMAHA_BASE_XML_UTILS_H_

#include <windows.h>
#include <objbase.h>
#include <msxml.h>
#include <atlstr.h>
#include <utility>
#include <vector>

#include "omaha/base/debug.h"
#include "omaha/base/utils.h"

namespace omaha {

// Creates a DOMDocument that disallows external definitions to be included and
// resolved as part of the XML document stream at parse time.
HRESULT CoCreateSafeDOMDocument(IXMLDOMDocument** my_xmldoc);

// xmlfile can be any specified encoding.
HRESULT LoadXMLFromFile(const wchar_t* xmlfile,
                        bool preserve_whitespace,
                        IXMLDOMDocument** xmldoc);

// xmlstring must be UTF-16 or UCS-2.
HRESULT LoadXMLFromMemory(const wchar_t* xmlstring,
                          bool preserve_whitespace,
                          IXMLDOMDocument** xmldoc);

// xmldata can be any raw data supported by xml parser
HRESULT LoadXMLFromRawData(const std::vector<byte>& xmldata,
                           bool preserve_whitespace,
                           IXMLDOMDocument** xmldoc);

// xmlfile is in encoding specified in the XML document.
HRESULT SaveXMLToFile(IXMLDOMDocument* xmldoc, const wchar_t * xmlfile);

// xmlstring is in UCS-2
HRESULT SaveXMLToMemory(IXMLDOMDocument* xmldoc, CString* xmlstring);

// buffer is in the encoding specified in the XML document.
HRESULT SaveXMLToRawData(IXMLDOMDocument* xmldoc, std::vector<byte>* buffer);

// Dealing with element/attribute names: the combination of a base name
// and a namespace URI is a fully-qualified XML name, or: XMLFQName.

// We can't just typedef a std::pair because we need proper comparison operators
// in case we want to stick a XMLFQName into a standard collection.
struct XMLFQName {
  XMLFQName();
  XMLFQName(const wchar_t* u, const wchar_t* b);
  ~XMLFQName();

  CString uri;
  CString base;
};

bool operator==(const XMLFQName& u, const XMLFQName& v);
bool operator!=(const XMLFQName& u, const XMLFQName& v);
bool operator< (const XMLFQName& u, const XMLFQName& v);
bool operator> (const XMLFQName& u, const XMLFQName& v);
bool operator<=(const XMLFQName& u, const XMLFQName& v);
bool operator>=(const XMLFQName& u, const XMLFQName& v);

bool EqualXMLName(const XMLFQName& u, const XMLFQName& v);
bool EqualXMLName(IXMLDOMNode* pnode, const XMLFQName& u);
bool EqualXMLName(const XMLFQName& u, IXMLDOMNode* pnode);

// Returns the FQ name from the node.
HRESULT GetXMLFQName(IXMLDOMNode* node, XMLFQName* name);

//
// Routines for dealing with fragments of DOM trees.
//
// Creates an XMLDOMNode of the given type with a given name and optional text.
HRESULT CreateXMLNode(IXMLDOMDocument* xmldoc,
                      int node_type,
                      const wchar_t* node_name,
                      const wchar_t* namespace_uri,
                      const wchar_t* text,
                      IXMLDOMNode** node_out);

// Adds newchild as an attribute node of xmlnode replacing existing
// attribute with same name.
HRESULT AddXMLAttributeNode(IXMLDOMNode* xmlnode, IXMLDOMAttribute* new_child);

// Adds name/value pair as an attribute node of xmlnode replacing
// existing attribute with same name.
HRESULT AddXMLAttributeNode(IXMLDOMElement* xmlelement,
                            const wchar_t* attribute_name,
                            const wchar_t* attribute_value);

// Adds name/value pair as an attribute node of xmlnode replacing
// existing attribute with same name.
// Can add attributes to nodes other than IXMLDOMElement.
// Can add attributes with non-null namespaces.
HRESULT AddXMLAttributeNode(IXMLDOMNode* xmlnode,
                            const wchar_t* attribute_namespace,
                            const wchar_t* attribute_name,
                            const wchar_t* attribute_value);

// Gets parse error information after a failed load.
HRESULT GetXMLParseError(IXMLDOMDocument* xmldoc,
                         IXMLDOMParseError** parse_error);

// Interprets parse error.
HRESULT InterpretXMLParseError(IXMLDOMParseError* parse_error,
                               HRESULT* error_code,
                               CString* message);

// Returns true if the specified attribute is in this node.
bool HasAttribute(IXMLDOMNode* node, const wchar_t* attr_name);

// Reads and parses attributes of nodes.
HRESULT ReadBooleanAttribute(IXMLDOMNode* node,
                             const wchar_t* attr_name,
                             bool* value);
HRESULT ReadIntAttribute(IXMLDOMNode* node,
                         const wchar_t* attr_name,
                         int* value);
HRESULT ReadGuidAttribute(IXMLDOMNode* node,
                          const wchar_t* attr_name,
                          GUID* value);
HRESULT ReadStringAttribute(IXMLDOMNode* node,
                            const wchar_t* attr_name,
                            CString* value);

// Reads an attribute as a BSTR, given the node and the name of the attribute.
// This is a helper for the other ReadXXXAttribute methods.
HRESULT ReadAttribute(IXMLDOMNode* node,
                      const wchar_t* attr_name,
                      BSTR* value);

// Reads the string value of a node element, either TEXT or CDATA.
HRESULT ReadStringValue(IXMLDOMNode* node, CString* value);

}  // namespace omaha

#endif  // OMAHA_BASE_XML_UTILS_H_

