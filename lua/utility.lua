local bit = require "bit32"

Auxiliary={}
aux=Auxiliary

function Auxiliary.AddSetCode(existing,new)
	local new_type = bit.band(new,0xfff)
	local new_subtype = bit.band(new,0xf000)
	local shift = 0
	local added = false
	--Look for a parent archetype in the existing setcode and replace the first one found
	while (not added) and bit.rshift(existing, shift) > 0 do
		local part = bit.band(bit.rshift(existing,shift),0xffff)

		local part_type = bit.band(part,0xfff)
		local part_subtype = bit.band(part,0xf000)
		if part_type == new_type then 
			if bit.band(part_subtype, new_subtype) == part_subtype then
				existing = existing - bit.lshift(part,shift)
				existing = existing + bit.lshift(new,shift)
				added = true
			elseif bit.band(part_subtype, new_subtype) == new_subtype then
				added = true
			end
		end
		shift = shift + 16
	end
	--If no parent archetype was replaced, add the new setcode as an additional archetype
	if not added then
		existing = bit.lshift(existing,16) + new
	end

	return existing
end
function Auxiliary.PendulumLevel(level,lscale,rscale)
	return tonumber(string.format('%x',lscale).."0"..string.format('%x',rscale).."000"..string.format("%x",level), 16)
end
function Auxiliary.SymEscape(text, dict)
	dict = dict or {}
	for sym,trans in pairs(dict) do
		text = text:gsub(sym,trans)
	end
	return text:gsub("<.->(.-)</.->", "%1")
end
function Auxiliary.ToLines(text)
	lines = {}
	for l in text:gmatch("[^\r\n]+") do
		table.insert(lines, l)
	end
	return lines
end
function Auxiliary.BuildCardDescription(type, ruleText, pendulumText, pendulumScale)
	local fullDesc = ""

	if bit.band(type,TYPE_PENDULUM) == TYPE_PENDULUM then
		fullDesc = "Pendulum Scale = "..pendulumScale
		if pendulumText ~= "" then
			fullDesc = fullDesc.."\n[ Pendulum Effect ]"
			fullDesc = fullDesc.."\n"..pendulumText
			fullDesc = fullDesc.."\n----------------------------------------"
			if bit.band(type,TYPE_NORMAL) == TYPE_NORMAL then
				fullDesc = fullDesc.."\n[ Flavor Text ]"
			else
				fullDesc = fullDesc.."\n[ Monster Effect ]"
			end
		end
	end
	if fullDesc ~= "" then fullDesc = fullDesc.."\n" end
	fullDesc = fullDesc..ruleText

	return fullDesc
end