/*
 * Copyright (C) 2018-2020 HERE Europe B.V.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 * License-Filename: LICENSE
 */

//
// Created by onur on 10/11/18.
//

#ifndef MOVETK_TRAJECTORY_TO_INTERFACE_H
#define MOVETK_TRAJECTORY_TO_INTERFACE_H

#include <iostream>
#include <vector>
#include <optional>
#include <GeographicLib/Geocentric.hpp>

#include "movetk/geom/GeometryInterface.h"
#include "movetk/utils/Requirements.h"
#include "movetk/geo/geo.h"

/**
 * Contains adapter functions to make Trajectory class work with agnostic geometry interface.
 */
namespace movetk {

// Requires GeometryTraits::MovetkPoint has dimension 2
template <class GeometryTraits, class LatIterator, class LonIterator, class OutputIterator,
        class LocalCoordRef,
        typename = movetk_core::requires_movetk_point<GeometryTraits,
                                                    typename OutputIterator::value_type>>
OutputIterator
to_projected_polyline(movetk_core::MakePoint<GeometryTraits> &make_point,
                      LatIterator lat_start, LatIterator lat_beyond, LonIterator lon_start,
                      OutputIterator polyline_first,
                      LocalCoordRef& ref)
{
    while(lat_start != lat_beyond) {
        auto projected_point = ref.project(*lat_start++, *lon_start++);
        *polyline_first++ = make_point(std::cbegin(projected_point), std::cend(projected_point));
    }
    return polyline_first;
}

template <class GeometryTraits, class LatIterator, class LonIterator, class OutputIterator>
OutputIterator
to_projected_polyline(movetk_core::MakePoint<GeometryTraits> &make_point,
                      LatIterator lat_start, LatIterator lat_beyond, LonIterator lon_start,
                      OutputIterator polyline_first)
{
    LocalCoordinateReference<typename GeometryTraits::NT> ref(*lat_start, *lon_start);
    return to_projected_polyline(make_point, lat_start, lat_beyond, lon_start, polyline_first, ref);
}

/**
 * Creates a vector of points from the given ranges of latitude and longitude also by converting them to
 * geocentric coordinates.
 * @tparam GeometryTraits
 * @tparam LatIterator
 * @tparam LonIterator
 * @tparam OutputIterator
 * @param make_point
 * @param lat_start
 * @param lat_beyond
 * @param lon_start
 * @param polyline_first
 * @return
 */
template <class GeometryTraits, class LatIterator, class LonIterator, class OutputIterator,
        typename = movetk_core::requires_movetk_point<GeometryTraits,
                                                    typename OutputIterator::value_type>>
OutputIterator
to_geocentered_polyline(movetk_core::MakePoint<GeometryTraits> &make_point,
                        LatIterator lat_start, LatIterator lat_beyond, LonIterator lon_start,
                        OutputIterator polyline_first) {
    const GeographicLib::Geocentric& earth = GeographicLib::Geocentric::WGS84();
    while(lat_start != lat_beyond) {
        *polyline_first++ = to_geocentric_coordinates(make_point, earth, *lat_start++, *lon_start++);
    }
    return polyline_first;
}


} // /namespace movetk

#endif //MOVETK_TRAJECTORY_TO_INTERFACE_H
