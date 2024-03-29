//
//  data.hpp
//  DataCpp
//
//  Created by Luis Finke on 8/18/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Any.hpp>
#include <fgl/data/ArrayList.hpp>
#include <fgl/data/LinkedList.hpp>
#include <fgl/data/Map.hpp>
#include <fgl/data/String.hpp>
#include <fgl/data/Data.hpp>
#include <fgl/data/Number.hpp>
#include <fgl/data/URL.hpp>
#include <fgl/data/Stringify.hpp>
#include <fgl/data/Optional.hpp>
#include <fgl/data/Iterator.hpp>
#include <fgl/data/Variant.hpp>
#include <fgl/data/SharedPtr.hpp>
#include <fgl/crypto/MD5.hpp>
#include <fgl/error/Error.hpp>
#include <fgl/time/Date.hpp>
#include <fgl/time/DateFormatter.hpp>
#include <fgl/time/TimeZone.hpp>
#include <fgl/util/PlatformChecks.hpp>
#include <fgl/util/ObjectiveCpp.h>
#include <fgl/util/JNI.h>
#ifdef JNIEXPORT
#include <fgl/data/JNIDataCpp.hpp>
#endif
