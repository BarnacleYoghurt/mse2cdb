local bit = require "bit32"

CardData={}
cd=CardData

subtypes['ritual'] = TYPE_RITUAL
subtypes['quickplay'] = TYPE_QUICKPLAY
subtypes['continuous'] = TYPE_CONTINUOUS
subtypes['equipment'] = TYPE_EQUIP
subtypes['field'] = TYPE_FIELD
subtypes["counter"] = TYPE_COUNTER

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
	local cardtype = data:GetChildValue("card type")

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
		local icon = aux.SymEscape(data:GetChildNode("extra data"):GetChildNode("yugioh-ygopro-anime"):GetChildValue("icon"))
		if subtypes[icon] then
			result = result + subtypes[icon]
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
	local dict={["<sym%-auto>%*</sym%-auto>"]="●"}
	
	local notes=data:GetChildFullContent("notes")

	return aux.SymEscape(notes:sub(1,notes:find("%-%-%-"),dict)
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