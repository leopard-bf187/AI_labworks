--[[
/*
*  Copyright (c) BytesForge 2022-2025. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @LeoParD
*
*  Description: main script file
*
*  Date: 14.07.2025
*/
]]

LogDbg(" ----------- MAIN.LUA  BEGIN  ----------- ")
LogDbg("Major:     " .. krystallic.Major)
LogDbg("Minor:     " .. krystallic.Minor)
LogDbg("Revision:  " .. krystallic.Revision)
LogDbg("Version:   " .. krystallic.Version)
LogDbg("Build:     " .. krystallic.BuildStr)
LogDbg("BuildDate: " .. krystallic.BuildTimeStamp)

local function Vector2Test()
	local v1 = CVector2(1.0, 0.0)
	local v2 = CVector2(0.0, 1.0)
	local v3 = v1 + v2
	LogDbg("Vector2: + " .. tostring(v3.x) .. " " .. tostring(v3.y))
	v3 = v1 - v2
	LogDbg("Vector2: - " .. tostring(v3.x) .. " " .. tostring(v3.y))
	v3 = v1 * 2.0
	LogDbg("Vector2: * " .. tostring(v3.x) .. " " .. tostring(v3.y))
	v3 = v1 / 4.0
	LogDbg("Vector2: / " .. tostring(v3.x) .. " " .. tostring(v3.y))
	LogDbg("Vector2: Length " .. tostring(v3:Length()))
	LogDbg("Vector2: InvLength " .. tostring(v3:InvLength()))
	LogDbg("Vector2: IsNaN " .. tostring(v3:IsNaN()))
	LogDbg("Vector2: IsInf " .. tostring(v3:IsInf()))
end

local function Vector3Test()
	local v1 = CVector3(1.0, 0.0, 0.0)
	local v2 = CVector3(0.0, 1.0, 0.0)
	local v3 = v1 + v2
	LogDbg("Vector3: + " .. tostring(v3.x) .. " " .. tostring(v3.y) .. " " .. tostring(v3.z))
	v3 = v1 - v2
	LogDbg("Vector3: - " .. tostring(v3.x) .. " " .. tostring(v3.y) .. " " .. tostring(v3.z))
	v3 = v1 * 2.0
	LogDbg("Vector3: * " .. tostring(v3.x) .. " " .. tostring(v3.y) .. " " .. tostring(v3.z))
	v3 = v1 / 4.0
	LogDbg("Vector3: / " .. tostring(v3.x) .. " " .. tostring(v3.y) .. " " .. tostring(v3.z))
	LogDbg("Vector3: Length " .. tostring(v3:Length()))
	LogDbg("Vector3: InvLength " .. tostring(v3:InvLength()))
	LogDbg("Vector3: IsNaN " .. tostring(v3:IsNaN()))
	LogDbg("Vector3: IsInf " .. tostring(v3:IsInf()))
end

local function Vector4Test()
	local v1 = CVector4(1.0, 0.0, 0.0, 1.0)
	local v2 = CVector4(0.0, 1.0, 0.0, 1.0)
	local v3 = v1 + v2
	LogDbg("Vector4: + " .. tostring(v3.x) .. " " .. tostring(v3.y) .. " " .. tostring(v3.z) .. " " .. tostring(v3.w))
	v3 = v1 - v2
	LogDbg("Vector4: - " .. tostring(v3.x) .. " " .. tostring(v3.y) .. " " .. tostring(v3.z) .. " " .. tostring(v3.w))
	v3 = v1 * 2.0
	LogDbg("Vector4: * " .. tostring(v3.x) .. " " .. tostring(v3.y) .. " " .. tostring(v3.z) .. " " .. tostring(v3.w))
	v3 = v1 / 4.0
	LogDbg("Vector4: / " .. tostring(v3.x) .. " " .. tostring(v3.y) .. " " .. tostring(v3.z) .. " " .. tostring(v3.w))
	LogDbg("Vector4: Length " .. tostring(v3:Length()))
	LogDbg("Vector4: InvLength " .. tostring(v3:InvLength()))
	LogDbg("Vector4: IsNaN " .. tostring(v3:IsNaN()))
	LogDbg("Vector4: IsInf " .. tostring(v3:IsInf()))
end

local function RunVectorTest(name, fn)
	local ok, err = pcall(fn)
	if not ok then
		LogErr(name .. ": " .. tostring(err))
	end
end

RunVectorTest("Vector2", Vector2Test)
RunVectorTest("Vector3", Vector3Test)
RunVectorTest("Vector4", Vector4Test)

LogMsg(" --- SCRIPT EXECUTION MSG --- ")
LogInf(" --- SCRIPT EXECUTION INF --- ")
LogDbg(" --- SCRIPT EXECUTION DBG --- ")
LogWrn(" --- SCRIPT EXECUTION WRN --- ")
LogErr(" --- SCRIPT EXECUTION ERR --- ")
LogFtl(" --- SCRIPT EXECUTION FTL --- ")
LogExc(" --- SCRIPT EXECUTION EXC --- ")
DebugPrint(" --- SCRIPT EXECUTION DEBUGPRINT --- ")
LogDbg(" ----------- MAIN.LUA   END   ----------- ")