//
//  URL.cpp
//  DataCpp
//
//  Created by Luis Finke on 1/4/22.
//  Copyright © 2022 Luis Finke. All rights reserved.
//

#include "URL.hpp"

namespace fgl {
	// code adapted from https://github.com/chmike/CxxUrl/blob/master/url.cpp

	static const uint8_t URL_chars_table[256] = {
		0,0,0,0, 0,0,0,0,     // NUL SOH STX ETX  EOT ENQ ACK BEL
		0,0,0,0, 0,0,0,0,     // BS  HT  LF  VT   FF  CR  SO  SI
		0,0,0,0, 0,0,0,0,     // DLE DC1 DC2 DC3  DC4 NAK SYN ETB
		0,0,0,0, 0,0,0,0,     // CAN EM  SUB ESC  FS  GS  RS  US
		0x00,0x01,0x00,0x00, 0x01,0x20,0x01,0x01, // SP ! " #  $ % & '
		0x01,0x01,0x01,0x01, 0x01,0x01,0x01,0x08, //  ( ) * +  , - . /
		0x01,0x01,0x01,0x01, 0x01,0x01,0x01,0x01, //  0 1 2 3  4 5 6 7
		0x01,0x01,0x04,0x01, 0x00,0x01,0x00,0x10, //  8 9 : ;  < = > ?
		0x02,0x01,0x01,0x01, 0x01,0x01,0x01,0x01, //  @ A B C  D E F G
		0x01,0x01,0x01,0x01, 0x01,0x01,0x01,0x01, //  H I J K  L M N O
		0x01,0x01,0x01,0x01, 0x01,0x01,0x01,0x01, //  P Q R S  T U V W
		0x01,0x01,0x01,0x00, 0x00,0x00,0x00,0x01, //  X Y Z [  \ ] ^ _
		0x00,0x01,0x01,0x01, 0x01,0x01,0x01,0x01, //  ` a b c  d e f g
		0x01,0x01,0x01,0x01, 0x01,0x01,0x01,0x01, //  h i j k  l m n o
		0x01,0x01,0x01,0x01, 0x01,0x01,0x01,0x01, //  p q r s  t u v w
		0x01,0x01,0x01,0x00, 0x00,0x00,0x01,0x00, //  x y z {  | } ~ DEL
		0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0
	};

	inline bool is_char(char c, std::uint8_t mask) {
		return (URL_chars_table[static_cast<unsigned char>(c)]&mask) != 0;
	}

	inline bool is_chars(const char* s, const char* e, std::uint8_t mask) {
		while(s!=e)
			if (!is_char(*s++,mask))
				return false;
		return true;
	}

	inline bool is_num(char c) {
		return c>='0'&&c<='9';
	}

	inline bool is_hexdigit(char c) {
		return is_num(c)||(c>='A'&&c<='F')||(c>='a'&&c<='f');
	}

	inline char get_hex_digit(char c) {
		if (c>='0'&&c<='9')
			return c-'0';
		if (c>='A'&&c<='F')
			return c-'A'+10;
		if (c>='a'&&c<='f')
			return c-'a'+10;
		return -1;
	}

	inline bool parse_while_uint(const char*& s, const char* e, uint32_t max) {
		if (s==e || !is_num(*s))
			return false;
		const char *t=s;
		uint32_t val = *t++-'0';
		if (val)
			while(t!=e && is_num(*t))
			   val=val*10+(*t++-'0');
		if (val>max)
			return false;
		s=t;
		return true;
	}

	inline bool is_ipv4(const char *s, const char *e) {
		size_t l=e-s;
		if (l<7 || l>254)
			return false;
		for (const char *p=s; p!=e; ++p)
			if (*p!='.'&&!is_num(*p))
				return false;
		return true;
	}

	bool is_ipv6(const char *s, const char *e) {
		size_t l=e-s;
		if (l<2 || l>254)
			return false;
		for (const char *p=s; p!=e; ++p)
			if (*p!=':'&&*p!='.'&&!is_hexdigit(*p))
				return false;
		return true;
	}

	inline const char* find_first_of(const char* s, const char* e, const char* q) {
		for(; s!=e; ++s)
			for(const char* t=q; *t; ++t)
				if (*s==*t)
					return s;
		return e;
	}

	inline const char* find_char(const char *s, const char *e, const char c) {
		while (s!=e && *s!=c)
			++s;
		return s;
	}



	String URL::encodeComponent(const std::string& str, Function<bool(char)> shouldEncodeChar, const std::map<char,char>& mappedChars) {
		std::ostringstream encoded;
		encoded.fill('0');
		encoded << std::hex;
		
		for(char c : str) {
			auto mappedCharIt = mappedChars.find(c);
			if(mappedCharIt != mappedChars.end()) {
				encoded << mappedCharIt->second;
			}
			else if(shouldEncodeChar(c)) {
				encoded << std::uppercase;
				encoded << '%' << std::setw(2) << int((unsigned char)c);
				encoded << std::nouppercase;
			}
			else {
				encoded << c;
			}
		}
		
		return encoded.str();
	}


	String URL::decodeComponent(const char* str_begin, const char* str_end, const std::map<char,char>& mappedChars) {
		String decoded;
		decoded.reserve(str_end - str_begin);
		
		auto from_hex = [](char ch) -> char {
			return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
		};
		for (auto it=str_begin, end=str_end; it != end; it++) {
			char c = (*it);
			auto mappedCharIt = mappedChars.find(c);
			if(mappedCharIt != mappedChars.end()) {
				decoded += mappedCharIt->second;
			}
			else if (c == '%' && it[1] && it[2]) {
				char h = (from_hex(it[1]) << 4) | from_hex(it[2]);
				decoded += h;
				it += 2;
			}
			else {
				decoded += c;
			}
		}
		return decoded;
	}


	String URL::encodeUserComponent(const std::string& str) {
		return encodeComponent(str, [](char c) { return is_char(c, 0x05); });
	}

	String URL::decodeUserComponent(const char* begin, const char* end) {
		return decodeComponent(begin, end);
	}

	String URL::decodeUserComponent(const std::string& str) {
		return decodeUserComponent(str.data(), str.data()+str.length());
	}


	String URL::encodePathPart(const std::string& str) {
		return encodeComponent(str, [](char c) { return is_char(c, 0x0F) || c == '/'; });
	}

	String URL::decodePathPart(const char* begin, const char* end) {
		return decodeComponent(begin, end);
	}

	String URL::decodePathPart(const std::string& str) {
		return decodePathPart(str.data(), str.data()+str.length());
	}


	String URL::encodeQueryKey(const std::string& str) {
		return encodeComponent(str, [](char c) {
			return c == '+' || c == '=' || c == '&' || c == ';'
				|| !is_char(c, 0x1F);
		}, {
			{ ' ', '+' }
		});
	}

	String URL::decodeQueryKey(const char* begin, const char* end) {
		return decodeComponent(begin, end, {
			{ '+', ' ' }
		});
	}

	String URL::decodeQueryKey(const std::string& str) {
		return decodeQueryKey(str.data(), str.data()+str.length());
	}


	String URL::encodeQueryValue(const std::string& str) {
		return encodeComponent(str, [](char c) {
			return c == '+' || c == '&' || c == ';'
				|| !is_char(c, 0x1F);
		}, {
			{ ' ', '+' }
		});
	}

	String URL::decodeQueryValue(const char* begin, const char* end) {
		return decodeComponent(begin, end, {
			{ '+', ' ' }
		});
	}

	String URL::decodeQueryValue(const std::string& str) {
		return decodeQueryValue(str.data(), str.data()+str.length());
	}


	bool URL::isSchemeValid(const char* begin, const char* end) {
		if(begin == end) {
			return true;
		}
		auto it = begin;
		// ensure first letter is alpha
		if(!std::isalpha(*it)) {
			return false;
		}
		it++;
		// validate remaining characters
		for(; it!=end; it++) {
			char c = *it;
			if(!(std::isalnum(c) || c == '+' || c == '-' || c == '.')) {
				return false;
			}
		}
		return true;
	}

	bool URL::isSchemeValid(const std::string& str) {
		return isSchemeValid(str.data(), str.data()+str.length());
	}

	bool URL::isHostValid(const char* begin, const char* end) {
		// host can be empty
		if(begin == end) {
			return true;
		}
		// detect ipv4
		if(is_ipv4(begin, end)) {
			return true;
		}
		if(*begin == '[' && *(end-1) == ']') {
			// detect ipvfuture
			if(*(begin+1) == 'v' && is_hexdigit(*(begin+2))) {
				return true;
			}
			// detect ipv6
			return is_ipv6(begin+1, end-1);
		}
		// detect valid hostname
		return is_chars(begin, end, 0x01);
	}

	bool URL::isHostValid(const std::string& str) {
		return isHostValid(str.data(), str.data()+str.length());
	}

	bool URL::isUsernameValid(const char* begin, const char* end) {
		return is_chars(begin, end, 0x21);
	}

	bool URL::isUsernameValid(const std::string& str) {
		return isUsernameValid(str.data(), str.data()+str.length());
	}

	bool URL::isPasswordValid(const char* begin, const char* end) {
		return is_chars(begin, end, 0x25);
	}

	bool URL::isPasswordValid(const std::string& str) {
		return isPasswordValid(str.data(), str.data()+str.length());
	}

	bool URL::isPortValid(const char* begin, const char* end) {
		// port can be empty
		if(begin == end) {
			return true;
		}
		// ensure full value is a uint
		return parse_while_uint(begin,end,65535) && begin == end;
	}

	bool URL::isPortValid(const std::string& str) {
		return isPortValid(str.data(), str.data()+str.length());
	}

	bool URL::isPathValid(const char* begin, const char* end) {
		return is_chars(begin, end, 0x2F);
	}

	bool URL::isPathValid(const std::string& str) {
		return isPathValid(str.data(), str.data()+str.length());
	}

	bool URL::isFragmentValid(const char* begin, const char* end) {
		return is_chars(begin, end, 0x3F);
	}

	bool URL::isFragmentValid(const std::string& str) {
		return isFragmentValid(str.data(), str.data()+str.length());
	}



	URL::URL(const std::string& str) {
		if (str.empty()) {
			throw std::invalid_argument("Empty string is not a valid URL");
		}
		
		const char *s=str.data(), *e=s+str.length();
		const char *scheme_b, *scheme_e, *user_b, *user_e, *host_b, *host_e,
				*port_b, *port_e, *path_b, *path_e, *query_b, *query_e,
				*fragment_b, *fragment_e;
		scheme_b=scheme_e=user_b=user_e=host_b=host_e=port_b=port_e=path_b=
				path_e=query_b=query_e=fragment_b=fragment_e=nullptr;

		const char *b=s, *p=find_first_of(b, e, ":/?#");
		if (p==e) {
			if (!is_chars(b, p, 0x2F)) {
				// path is invalid
				throw std::invalid_argument("Path '"+std::string(b,p)+"' in '"+std::string(s,e-s)+"' is invalid");
			}
			path_b=b;
			path_e=e;
		} else {
			// get schema if any
			if (*p==':') {
				if (!isSchemeValid(b, p)) {
					// scheme is invalid
					throw std::invalid_argument("Scheme in '"+std::string(s,e-s)+"' is invalid");
				}
				scheme_b=b;
				scheme_e=p;
				p=find_first_of(b=p+1, e, "/?#");
			}
			// get authority if any
			if (p!=e && *p=='/' && (e-b)>1 && b[0]=='/' && b[1]=='/') {
				const char *ea=find_first_of(b+=2, e, "/?#"); // locate end of authority
				p=find_char(b, ea, '@');
				// get user info if any
				if (p!=ea) {
					if (!is_chars(b, p, 0x25)) {
						// user info is invalid
						throw std::invalid_argument("User info in '"+std::string(s,e-s)+"' is invalid");
					}
					user_b=b;
					user_e=p;
					b=p+1;
				}
				// get IP literal if any
				if (*b=='[') {
					// locate end of IP literal
					p=find_char(b+1, ea, ']');
					if (*p!=']') {
						// missing ending ]
						throw std::invalid_argument("Missing ] in '"+std::string(s,e-s)+"'");
					}
					if(!isHostValid(b, p+1)) {
						// host is invalid
						throw std::invalid_argument("Host address in '"+std::string(s,e-s)+"' is invalid");
					}
					// TODO validate ipv6 address in brackets
					host_b=b;
					host_e=p+1;
					b=p+1;
				}
				// get host
				else {
					p=find_char(b, ea, ':');
					if (!isHostValid(b, p)) {
						// host is invalid
						throw std::invalid_argument("Host address in '"+std::string(s,e-s)+"' is invalid");
					}
					host_b=b;
					host_e=p;
					b=p;
				}
				// get port if any
				if (b!=ea && *b==':') {
					if (!isPortValid(++b, ea)) {
						// port is invalid
						throw std::invalid_argument("Port '"+std::string(b,ea-b)+"' in '"+std::string(s,e-s)+"' is invalid");
					}
					port_b=b;
					port_e=ea;
				}
				b=ea;
			}
			// get path
			p=find_first_of(b,e,"?#");
			if (!isPathValid(b, p)) {
				// path is invalid
				throw std::invalid_argument("Path '"+std::string(b,p)+"' in '"+std::string(s,e-s)+"' is invalid");
			}
			path_b=b;
			path_e=p;
			if (p!=e && *p=='?') {
				p=find_char(b=p+1,e,'#');
				query_b=b;
				query_e=p;
				// TODO validate query
			}
			if (p!=e && *p=='#') {
				if (!isFragmentValid(p+1, e)) {
					// fragment is invalid
					throw std::invalid_argument("Fragment '"+std::string(p+1,e)+"' in '"+std::string(s,e-s)+"' is invalid");
				}
				fragment_b=p+1;
				fragment_e=e;
			}
		}
		
		if(scheme_b) {
			_scheme = String(scheme_b, scheme_e);
		}
		if(user_b) {
			auto colon = find_char(user_b, user_e, ':');
			if(colon != user_e) {
				_username = decodeUserComponent(user_b, colon);
				_password = decodeUserComponent(colon+1, user_e);
			}
			else {
				_username = decodeUserComponent(user_b, user_e);
			}
		}
		if(host_b) {
			_host = decodeComponent(host_b, host_e);
		}
		if(port_b && port_b != port_e) {
			_port = String(port_b, port_e).toArithmeticValue<int>();
		}
		if(query_b) {
			_query = String(query_b, query_e);
		}
		if(fragment_b) {
			_fragment = String(fragment_b, fragment_e);
		}
	}



	URL::URL(Components url) {
		if(url.scheme) {
			if(!isSchemeValid(url.scheme.value())) {
				throw std::invalid_argument("URL scheme "+url.scheme.value()+" is invalid");
			}
			_scheme = url.scheme;
		}
		if(url.host) {
			if(!isHostValid(url.host.value())) {
				throw std::invalid_argument("URL host "+url.host.value()+" is invalid");
			}
			_host = url.host;
		}
		else {
			if(url.username) {
				throw std::invalid_argument("Cannot create URL with a username without also providing a hostname");
			}
			if(url.password) {
				throw std::invalid_argument("Cannot create URL with a password without also providing a hostname");
			}
			if(url.port) {
				throw std::invalid_argument("Cannot create URL with a port without also providing a hostname");
			}
		}
		if(url.username) {
			if(!isUsernameValid(url.username.value())) {
				throw std::invalid_argument("URL username "+url.username.value()+" is invalid");
			}
			_username = url.username;
		}
		if(url.password) {
			if(!isPasswordValid(url.password.value())) {
				throw std::invalid_argument("URL password is invalid");
			}
			_password = url.password;
		}
		if(url.port) {
			_port = url.port;
		}
		if(!url.path.empty()) {
			if(!isPathValid(url.path)) {
				throw std::invalid_argument("URL path "+url.path+" is invalid");
			}
			_path = url.path;
		}
		if(url.query) {
			_query = url.query;
		}
		if(url.fragment) {
			if(!isFragmentValid(_fragment.value())) {
				throw std::invalid_argument("URL fragment "+url.fragment.value()+" is invalid");
			}
			_fragment = url.fragment;
		}
	}



	Optional<URL> parse(const std::string& str) {
		try {
			return URL(str);
		}
		catch(const std::invalid_argument&) {
			return std::nullopt;
		}
	}



	String URL::toString() const {
		std::ostringstream urlString;
		if(_scheme.hasValue()) {
			urlString << _scheme.value() << ':';
		}
		if(_host.hasValue()) {
			urlString << "//";
			bool includeAtSymbol = false;
			if(_username.hasValue()) {
				urlString << encodeUserComponent(_username.value());
				includeAtSymbol = true;
			}
			if(_password.hasValue()) {
				urlString << ':' << encodeUserComponent(_password.value());
				includeAtSymbol = true;
			}
			if(includeAtSymbol) {
				urlString << '@';
			}
			urlString << _host.value();
			if(_port.hasValue()) {
				urlString << ':' << std::to_string(_port.value());
			}
			if(!_path.empty()) {
				if(_path[0] != '/') {
					urlString << '/';
				}
				urlString << _path;
			}
		}
		else {
			if(_username.hasValue()) {
				throw std::invalid_argument("Cannot create URL with a username without also providing a hostname");
			}
			if(_password.hasValue()) {
				throw std::invalid_argument("Cannot create URL with a password without also providing a hostname");
			}
			if(_port.hasValue()) {
				throw std::invalid_argument("Cannot create URL with a port without also providing a hostname");
			}
			urlString << _path;
		}
		if(_query.hasValue()) {
			urlString << '?' << _query.value();
		}
		if(_fragment.hasValue()) {
			urlString << '#' << _fragment.value();
		}
		return urlString.str();
	}



	const Optional<String>& URL::scheme() const {
		return _scheme;
	}

	const Optional<String>& URL::username() const {
		return _username;
	}

	const Optional<String>& URL::password() const {
		return _password;
	}

	const Optional<String>& URL::host() const {
		return _host;
	}

	const Optional<uint16_t>& URL::port() const {
		return _port;
	}

	const String& URL::path() const {
		return _path;
	}

	const Optional<String>& URL::query() const {
		return _query;
	}

	const Optional<String>& URL::fragment() const {
		return _fragment;
	}


	LinkedList<URL::QueryItem> URL::queryItems() const {
		auto items = LinkedList<QueryItem>();
		if(!_query || _query->empty()) {
			return items;
		}
		auto begin = _query.value().data();
		auto end = begin + _query.value().length();
		auto it = begin;
		auto lastStart = begin;
		while(it != end) {
			if(*it == '&') {
				items.pushBack(QueryItem::parse(lastStart, it));
				it++;
				lastStart = it;
			}
			else {
				it++;
			}
		}
		if(lastStart != it) {
			items.pushBack(QueryItem::parse(lastStart, it));
		}
		return items;
	}

	void URL::setQueryItems(const ArrayList<QueryItem>& queryItems) {
		if(queryItems.empty()) {
			_query = std::nullopt;
		} else {
			_query = makeQueryString(queryItems);
		}
	}


	bool URL::pathIsRelative() const {
		if(_path.empty() || _path[0] == '/') {
			return false;
		}
		return true;
	}

	bool URL::pathHasTrailingSlash() const {
		if(!_path.endsWith("/")) {
			return false;
		}
		if(_path.length() > 1) {
			return true;
		} else {
			if(_host) {
				return true;
			} else {
				// if there's no host, a path of "/" would not be a trailing slash
				return false;
			}
		}
	}

	void URL::setPathHasTrailingSlash(bool hasTrailingSlash) {
		if(hasTrailingSlash) {
			if(_path.endsWith("/")) {
				return;
			}
			if(_host || _path.length() > 0) {
				_path += "/";
			}
		} else {
			if(!_path.endsWith("/")) {
				return;
			}
			if(_host || _path.length() > 1) {
				_path = _path.substring(0, _path.length() - 1);
			}
		}
	}

	LinkedList<String> URL::pathParts() const {
		auto parts = LinkedList<String>();
		if(_path.empty()) {
			return parts;
		}
		auto begin = _path.data();
		auto end = begin + _path.length();
		auto it = begin;
		if(*it == '/') {
			it++;
		}
		auto lastStart = it;
		while(it != end) {
			if(*it == '/') {
				parts.pushBack(decodePathPart(lastStart, it));
				it++;
				lastStart = it;
			}
			else {
				it++;
			}
		}
		if(lastStart != it) {
			parts.pushBack(decodePathPart(lastStart, it));
		}
		return parts;
	}

	void URL::setPathParts(const ArrayList<String>& parts, bool relative) {
		_path = (relative ? "" : "/") + String::join(parts.map([](auto& part) { return encodePathPart(part); }), "/");
	}



	URL::QueryItem URL::QueryItem::parse(const char* begin, const char* end) {
		auto separatorIt = find_char(begin, end, '=');
		if(separatorIt == end) {
			return QueryItem{
				.key = decodeQueryKey(begin, end)
			};
		}
		return QueryItem{
			.key = decodeQueryKey(begin, separatorIt),
			.value = decodeQueryValue(separatorIt+1, end)
		};
	}

	URL::QueryItem URL::QueryItem::parse(const std::string& str) {
		return parse(str.data(), str.data() + str.length());
	}

	String URL::QueryItem::toString() const {
		if(!value.hasValue()) {
			return encodeQueryKey(key);
		}
		return String::join({ encodeQueryKey(key), "=", encodeQueryValue(value.value()) });
	}

	String URL::makeQueryString(const ArrayList<QueryItem>& items) {
		return String::join(items.map([](auto& item) {
			return item.toString();
		}), "&");
	}

	String URL::makeQueryString(const std::map<String,String>& items) {
		LinkedList<String> itemStrings;
		for(auto& pair : items) {
			itemStrings.pushBack(QueryItem{ .key = pair.first, .value = pair.second }.toString());
		}
		return String::join(itemStrings, "&");
	}

	LinkedList<URL::QueryItem> URL::parseQueryItems(const String& queryString) {
		auto items = LinkedList<QueryItem>();
		if(queryString.empty()) {
			return items;
		}
		auto begin = queryString.data();
		auto end = begin + queryString.length();
		auto it = begin;
		auto lastStart = begin;
		while(it != end) {
			if(*it == '&') {
				items.pushBack(QueryItem::parse(lastStart, it));
				it++;
				lastStart = it;
			}
			else {
				it++;
			}
		}
		if(lastStart != it) {
			items.pushBack(QueryItem::parse(lastStart, it));
		}
		return items;
	}

	Map<String,String> URL::parseQueryItemMap(const String& queryString) {
		auto items = Map<String,String>();
		if(queryString.empty()) {
			return items;
		}
		auto begin = queryString.data();
		auto end = begin + queryString.length();
		auto it = begin;
		auto lastStart = begin;
		while(it != end) {
			if(*it == '&') {
				auto item = QueryItem::parse(lastStart, it);
				items[item.key] = item.value.valueOr(String());
				it++;
				lastStart = it;
			}
			else {
				it++;
			}
		}
		if(lastStart != it) {
			auto item = QueryItem::parse(lastStart, it);
			items[item.key] = item.value.valueOr(String());
		}
		return items;
	}
}
