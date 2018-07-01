CardData={}
cd=CardData

function CardData.helloWorld()
	return "Hello world";
end

function CardData.id(data)
	return data:GetChildValue("gamecode")
end
function CardData.ot(data)
	return 1
end
function CardData.alias(data)
	return 0
end
function CardData.setcode(data)
	local setcode = 0x0
	local name = cd.name(data)
	local desc = cd.desc(data)
	
	for archName,archCode in pairs(archetypes) do
		if name:find(archName) or desc:find(string.format(treatedAs, archName)) then
			if setcode > 0 then
				return aux.DoubleSetcode(setcode,archCode)
			else
				setcode = archCode
			end
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
		local level = aux.SymEscape(data:GetChildValue("level"))
		if level:find("Spell") then
			result = result + TYPE_SPELL
		elseif level:find("Trap") then
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
			result = result + subtypes[level:sub(-1)]
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
	
	--local isPendulum = (bit.band(cd.type(data), TYPE_PENDULUM) > 0)
	--if isPendulum then
	--	local lscale = data:GetChildValue("blue scale")
	--	local rscale = data:GetChildValue("right scale")
	--	result = aux.PendulumLevel(result,lscale,rscale)
	--end
	return result
end
function CardData.race(data)
	return races[data:GetValue("type 1")]
end
function CardData.attribute(data)
	return attributes[data:GetValue("attribute")]
end

function CardData.name(data)
	return data:GetChildValue("name")
end
function CardData.desc(data)
	return data:GetChildFullContent("rule text")
end
function CardData.strList(data)
	local notes = aux.GetLines(data:GetMultilineValue("notes"))
	local strs = {}
	local strIndex = 1
	for note in notes do
		if note:sub(1,2) == "{{" and note:sub(-2) == "}}" then
			strs["str"..strIndex] = note:gsub("{{(.*)}}", "%1")
		end
	end
	return strs
end