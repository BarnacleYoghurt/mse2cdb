local bit = require "bit32"

CardData={}
cd=CardData

function CardData.helloWorld()
	return "Hello world";
end

function CardData.id(data)
	return data:GetChildValue("gamecode")
end
function CardData.ot(data)
	return 3
end
function CardData.alias(data)
	return 0
end
function CardData.setcode(data)
	local setcode = 0x0
	local name = cd.name(data)
	local desc = cd.desc(data)

	for archName,archCode in pairs(archetypes) do
		if name:find(archName) or desc:find(treatedAsPattern(archName)) then
			setcode = aux.AddSetCode(setcode,archCode)
		end
	end
	return setcode
end
function CardData.type(data)
	local result = 0x0
	local cardframe = data:GetChildNode("extra data"):GetChildNode("yugioh-standard-extra"):GetChildValue("card frame")
	if cardframe:find("token") then
		result = TYPE_MONSTER + TYPE_NORMAL + TYPE_TOKEN
	else
		local cardtype = data:GetChildValue("card type")
		local level = aux.SymEscape(data:GetChildValue("level"))
		if cardtype:find("spell") then
			result = result + TYPE_SPELL
		elseif cardtype:find("trap") then
			result = result + TYPE_TRAP
		else
			result = result + TYPE_MONSTER
		end
		
		if result == TYPE_MONSTER then
			local nextType = 2
			local hasSubtype = false
			while data:GetChildValue("type "..nextType) ~= "" do
				local escaped = aux.SymEscape(data:GetChildValue("type "..nextType))
				if subtypes[escaped] then
					hasSubtype = true
					result = result + subtypes[escaped]
				end
				nextType = nextType + 1
			end
			if not hasSubtype then
				result = result + TYPE_NORMAL
			end
		else
			if subtypes[level:sub(-1)] then
				result = result + subtypes[level:sub(-1)]
			end
		end
	end
	return result
end
function CardData.atk(data)
	return data:GetChildValue("attack")
end
function CardData.def(data)
	return data:GetChildValue("defense")
end
function CardData.level(data)
	local result = data:GetChildValue("level"):gsub("[^\\*]", ""):len()
	
	local isPendulum = (bit.band(cd.type(data), TYPE_PENDULUM) > 0)
	if isPendulum then
		local lscale = data:GetChildValue("pendulum scale 1")
		if lscale == "" then
			lscale = "0"
		end
		local rscale = data:GetChildValue("pendulum scale 2")
		if rscale == "" then
			rscale = "0"
		end

		result = aux.PendulumLevel(result,lscale,rscale)
    end
	return result
end
function CardData.race(data)
	return races[aux.SymEscape(data:GetChildValue("type 1"))]
end
function CardData.attribute(data)
	return attributes[data:GetChildValue("attribute")]
end
function CardData.category(data)
	return 0
end

function CardData.name(data)
	return data:GetChildValue("name")
end
function CardData.desc(data)
	local descDict = {["<sym%-auto>%*</sym%-auto>"]="●"}
	
	local cardType=cd.type(data)
	
	local desc = aux.SymEscape(data:GetChildFullContent("rule text"),descDict)
	
	local fullDesc = ""
	
	if bit.band(cardType,TYPE_PENDULUM) == TYPE_PENDULUM then
		local _,_,scale = string.format("%x", cd.level(data)):find("(%d-)0.*")
		fullDesc = "Pendulum Scale = "..scale
		local pendulumDesc = aux.SymEscape(data:GetChildFullContent("pendulum text"),descDict)
		if pendulumDesc ~= "" then
			fullDesc = fullDesc.."\n[ Pendulum Effect ]"
			fullDesc = fullDesc.."\n"..pendulumDesc
			fullDesc = fullDesc.."\n----------------------------------------"
			if bit.band(cardType,TYPE_NORMAL) == TYPE_NORMAL then
				fullDesc = fullDesc.."\n[ Flavor Text ]"
			else
				fullDesc = fullDesc.."\n[ Monster Effect ]"
			end
		end
	end
	if fullDesc ~= "" then fullDesc = fullDesc.."\n" end
	fullDesc = fullDesc..desc
	
	return fullDesc
end
function CardData.str(data)
	local notes = aux.ToLines(data:GetChildFullContent("notes"))
	local strs = {}

	for k,note in pairs(notes) do
		if note:sub(1,2) == "{{" and note:sub(-2) == "}}" then
			local content = note:gsub("{{(.*)}}", "%1")
			table.insert(strs, content)
		end
	end
	return strs
end