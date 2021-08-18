//
// Copyright (C) OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#ifndef __INET_PTR_
#define __INET_PTR_

template<class T>
using Ptr = std::shared_ptr<T>;

template<class T, typename... Args>
Ptr<T> makeShared(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<class T, class U>
Ptr<T> staticPtrCast(const Ptr<U>& r)
{
    return std::static_pointer_cast<T>(r);
}

template<class T, class U>
Ptr<T> dynamicPtrCast(const Ptr<U>& r)
{
    return std::dynamic_pointer_cast<T>(r);
}

template<class T, class U>
Ptr<T> constPtrCast(const Ptr<U>& r)
{
    return std::const_pointer_cast<T>(r);
}

#endif