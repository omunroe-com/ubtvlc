--[[
 Gets an artwork from amazon

 $Id$
 Copyright © 2007-2010 the VideoLAN team

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
--]]

function try_query(query)
    local s = vlc.stream( query )
    if not s then return nil end
    local page = s:read( 65653 )

    -- FIXME: multiple results may be available
    _, _, asin = string.find( page, "<asin>(%w+)</asin>" )
    if asin then
        return "http://images.amazon.com/images/P/"..asin..".01._SCLZZZZZZZ_.jpg"
    end
    vlc.msg.dbg("ASIN not found")
    return nil
end

-- Return the mbid for first release
function try_release(query)
    local s = vlc.stream( query )
    if not s then return nil end
    local page = s:read( 65653 )

    -- FIXME: multiple results may be available and the first one is not
    -- guaranteed to have asin, so if it doesnt, we wouldnt get any art
    _, _, releaseid = string.find( page, "<release id=\"([%x%-]-)\">" )
    if releaseid then
        return releaseid
    end
    return nil
end

-- Return the artwork
function fetch_art()
    local meta = vlc.item:metas()

    if meta["Listing Type"] == "radio"
    or meta["Listing Type"] == "tv"
    then return nil end

    if meta["artist"] and meta["album"] then
        query = "artist:\"" .. meta["artist"] .. "\" AND release:\"" .. meta["album"] .. "\""
        relquery = "http://mb.videolan.org/ws/2/release/?query=" .. vlc.strings.encode_uri_component( query )
        return try_query( relquery )
    elseif meta["artist"] and meta["title"] then
        query = "artist:\"" .. meta["artist"] .. "\" AND recording:\"" .. meta["title"] .. "\""
        recquery = "http://mb.videolan.org/ws/2/recording/?query=" .. vlc.strings.encode_uri_component( query )
        releaseid = try_release( recquery )
        if releaseid then
            relquery = "http://mb.videolan.org/ws/2/release/" .. releaseid
            return try_query( relquery )
        else
            return nil
        end
    end
end
