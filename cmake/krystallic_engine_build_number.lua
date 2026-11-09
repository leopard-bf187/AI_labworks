local epochStr = arg[1] or "2025-12-06"

local function GetScriptDirectory()
    local scriptPath = arg[0] or ""
    local directory = scriptPath:match("^(.*[\\/])")

    return directory or ""
end

local function ParseDate(str)
    local year, month, day = str:match("^(%d+)%-(%d+)%-(%d+)$")

    if not year then
        error("Invalid date format: " .. tostring(str) .. ". Expected YYYY-MM-DD.")
    end

    return tonumber(year), tonumber(month), tonumber(day)
end

local function DateToDays(year, month, day)
    if month <= 2 then
        year = year - 1
    end

    local era = year >= 0 and math.floor(year / 400) or math.floor((year - 399) / 400)
    local yearOfEra = year - era * 400
    local adjustedMonth = month > 2 and month - 3 or month + 9
    local dayOfYear = math.floor((153 * adjustedMonth + 2) / 5) + day - 1
    local dayOfEra = yearOfEra * 365 + math.floor(yearOfEra / 4) - math.floor(yearOfEra / 100) + dayOfYear

    return era * 146097 + dayOfEra
end

local function CompareDates(a, b)
    local aYear, aMonth, aDay = ParseDate(a)
    local bYear, bMonth, bDay = ParseDate(b)
    local aDays = DateToDays(aYear, aMonth, aDay)
    local bDays = DateToDays(bYear, bMonth, bDay)

    if aDays < bDays then
        return -1
    end

    if aDays > bDays then
        return 1
    end

    return 0
end

local function GetBuildOffset(offsets, date)
    local offset = 0

    for _, entry in ipairs(offsets) do
        if CompareDates(date, entry.from) >= 0 then
            offset = entry.offset
        else
            break
        end
    end

    return offset
end

local function ReadBuildNumber(path)
    local file = io.open(path, "r")

    if not file then
        return nil
    end

    local value = file:read("*l")
    file:close()

    local buildNumber = tonumber(value)

    if not buildNumber then
        error("Invalid build number in file: " .. path)
    end

    return buildNumber
end

local function WriteBuildNumber(path, buildNumber)
    local file, err = io.open(path, "w")

    if not file then
        error("Failed to create build file '" .. path .. "': " .. tostring(err))
    end

    file:write(tostring(buildNumber))
    file:write("\n")
    file:close()
end

local function FormatTimestamp(now)
    return string.format("%04d-%02d-%02d %02d:%02d:%02d", now.year, now.month, now.day, now.hour, now.min, now.sec)
end

local function TimestampToShort(timestamp)
    local year, month, day, hour, minute, second = timestamp:match("^(%d%d%d%d)%-(%d%d)%-(%d%d) (%d%d):(%d%d):(%d%d)$")

    if not year then
        error("Invalid build timestamp: " .. tostring(timestamp) .. ". Expected YYYY-MM-DD HH:MM:SS.")
    end

    return string.format("%02d%s%s-%s%s", tonumber(year) % 100, month, day, hour, minute)
end

local function ReadBuildTimestamp(path)
    local file = io.open(path, "r")

    if not file then
        return nil
    end

    local timestamp = file:read("*l")
    file:close()

    if not timestamp or timestamp == "" then
        error("Invalid build timestamp in file: " .. path)
    end

    TimestampToShort(timestamp)

    return timestamp
end

local function WriteBuildTimestamp(path, timestamp)
    local file, err = io.open(path, "w")

    if not file then
        error("Failed to create build timestamp file '" .. path .. "': " .. tostring(err))
    end

    file:write(timestamp)
    file:write("\n")
    file:close()
end

local function PrintBuildMetadata(buildNumber, timestamp)
    print(buildNumber)
    print(timestamp)
    print(TimestampToShort(timestamp))
end

local scriptDirectory = GetScriptDirectory()
local buildFile = scriptDirectory .. "krystallic_engine_build_number.txt"
local timestampFile = scriptDirectory .. "krystallic_engine_build_timestamp.txt"
local offsetsFile = scriptDirectory .. "krystallic_engine_build_number_offsets.lua"

local offsetsChunk, offsetsError = loadfile(offsetsFile)

if not offsetsChunk then
    error("Failed to load build offsets file '" .. offsetsFile .. "': " .. tostring(offsetsError))
end

local buildOffsets = offsetsChunk()

if type(buildOffsets) ~= "table" then
    error("Build offsets file must return a table.")
end

local buildNumber = ReadBuildNumber(buildFile)

if buildNumber then
    local buildTimestamp = ReadBuildTimestamp(timestampFile)

    if not buildTimestamp then
        buildTimestamp = FormatTimestamp(os.date("*t"))
        WriteBuildTimestamp(timestampFile, buildTimestamp)
    end

    PrintBuildMetadata(buildNumber, buildTimestamp)
    return
end

local epochYear, epochMonth, epochDay = ParseDate(epochStr)
local now = os.date("*t")
local currentDate = string.format("%04d-%02d-%02d", now.year, now.month, now.day)
local epochDays = DateToDays(epochYear, epochMonth, epochDay)
local currentDays = DateToDays(now.year, now.month, now.day)
local rawBuildNumber = currentDays - epochDays

if rawBuildNumber < 0 then
    error("Current date is earlier than build epoch.")
end

local offset = GetBuildOffset(buildOffsets, currentDate)
buildNumber = rawBuildNumber + offset

local buildTimestamp = FormatTimestamp(now)

WriteBuildNumber(buildFile, buildNumber)
WriteBuildTimestamp(timestampFile, buildTimestamp)

PrintBuildMetadata(buildNumber, buildTimestamp)
