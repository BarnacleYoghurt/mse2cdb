CardData={}
cd=CardData

function CardData.helloWorld()
	return "Hello world";
end

function CardData.id(data)
	return data:GetChildValue("gamecode")
end
function CardData.name(data)
	return data:GetValue("name")
end
function CardData.desc(data)
	return data:GetMultilineValue("rule text")
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
function CardData.setcode(data)
	local setcode = 0x0
	local name = cd.name(data)
	local desc = cd.desc(data)
	
	for archName,archCode in pairs(setcodes) do
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
	
	local cardframe = data:GetMultilineValue("extra data"):GetMultilineValue("yugioh-standard-extra"):GetValue("card frame")
	if cardframe:find("token") then
		result = TYPE_MONSTER + TYPE_NORMAL + TYPE_TOKEN
	else
		local level = aux.SymEscape(data:GetValue("level"))
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
			while data:GetValue("type "..nextType) do
				if subtype[data:GetValue("type"..nextType)] then
					hasSubtype = true
					result = result + subtype[data:GetValue("type"..nextType)]
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
	return data:GetValue("attack")
end
function CardData.def(data)
	return data:GetValue("defense")
end
function CardData.level(data)
	local result = data:GetValue("level"):gsub("[^\\*]", ""):len()
	
	local isPendulum = (bit.band(cd.type(data), TYPE_PENDULUM) > 0)
	if isPendulum then
		local lscale = data:GetValue("blue scale")
		local rscale = data:GetValue("right scale")
		result = aux.PendulumLevel(result,lscale,rscale)
	end
	return result
end
function CardData.race(data)
	return races[data:GetValue("type 1")]
end
function CardData.attribute(data)
	return attributes[data:GetValue("attribute")]
end