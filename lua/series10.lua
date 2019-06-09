local bit = require "bit32"

CardData={}
cd=CardData

subtypes['#'] = TYPE_RITUAL
subtypes['$'] = TYPE_QUICKPLAY
subtypes['%'] = TYPE_CONTINUOUS
subtypes['+'] = TYPE_EQUIP
subtypes['&'] = TYPE_FIELD
subtypes['!'] = TYPE_COUNTER

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
      if not desc:find(notTreatedAsPattern(archName)) then
        setcode = aux.AddSetCode(setcode,archCode)
      end
		end
	end
	return setcode
end
function CardData.type(data)
	local result = 0x0
	local cardtype = data:GetChildValue("card type")
	if cardtype:find("token") then
		result = TYPE_MONSTER + TYPE_NORMAL + TYPE_TOKEN
	else
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
			local level = aux.SymEscape(data:GetChildValue("level"))
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
	local type=cd.type(data)
	local isLink = (bit.band(type, TYPE_LINK) > 0)
	
	if isLink then
		local directions = {
			["DL"] = LINK_MARKER_BOTTOM_LEFT,
			["Down"] = LINK_MARKER_BOTTOM,
			["DR"] = LINK_MARKER_BOTTOM_RIGHT,
			["Left"] = LINK_MARKER_LEFT,
			["Right"] = LINK_MARKER_RIGHT,
			["UL"] = LINK_MARKER_TOP_LEFT,
			["Up"] = LINK_MARKER_TOP,
			["UR"] = LINK_MARKER_TOP_RIGHT
		}
		local markers = 0x0
		
		for k,v in pairs(directions) do
			if data:GetChildValue("Link Marker "..k)=="yes" then
				markers = markers + v
			end
		end
		
		return markers
	else
		return data:GetChildValue("defense")
	end
end
function CardData.level(data)
	local result = 0
	local type=cd.type(data)
	
	local isLink = (bit.band(type, TYPE_LINK) > 0)
	if isLink then
		result = data:GetChildValue("link number")
	else
		result = data:GetChildValue("level"):gsub("[^\\*]", ""):len()
	end
	
	local isPendulum = (bit.band(type, TYPE_PENDULUM) > 0)
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
	local dict={["<sym%-auto>%*</sym%-auto>"]="●"}
	
	local type=cd.type(data)
	local desc = aux.SymEscape(data:GetChildFullContent("rule text"),dict)
	local pendulumDesc = aux.SymEscape(data:GetChildFullContent("pendulum text"),dict)
	local _,_,scale = string.format("%x", cd.level(data)):find("(%d-)0.*")

	return aux.BuildCardDescription(type, desc, pendulumDesc, scale)
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