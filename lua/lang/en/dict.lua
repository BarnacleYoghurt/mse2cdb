races = {
	['Warrior'] = RACE_WARRIOR,
	['Spellcaster'] = RACE_SPELLCASTER,
	['Fairy'] = RACE_FAIRY,
	['Fiend'] = RACE_FIEND,
	['Zombie'] = RACE_ZOMBIE,
	['Zombie'] = RACE_ZOMBIE,
	['Machine'] = RACE_MACHINE,
	['Aqua'] = RACE_AQUA,
	['Pyro'] = RACE_PYRO,
	['Rock'] = RACE_ROCK,
	['Winged Beast'] = RACE_WINDBEAST,
	['Plant'] = RACE_PLANT,
	['Insect'] = RACE_INSECT,
	['Thunder'] = RACE_THUNDER,
	['Dragon'] = RACE_DRAGON,
	['Beast'] = RACE_BEAST,
	['Beast-Warrior'] = RACE_BEASTWARRIOR,
	['Dinosaur'] = RACE_DINOSAUR,
	['Fish'] = RACE_FISH,
	['Sea Serpent'] = RACE_SEASERPENT,
	['Reptile'] = RACE_REPTILE,
	['Psychic'] = RACE_PSYCHO,
	['Divine-beast'] = RACE_DEVINE,
	['Creator God '] = RACE_CREATORGOD,
	['Cyberse'] = RACE_CYBERSE
}
subtypes = {
	['Effect'] = TYPE_EFFECT,
	['Fusion'] = TYPE_FUSION,
	['Ritual'] = TYPE_RITUAL,
	['Spirit'] = TYPE_SPIRIT,
	['Union'] = TYPE_UNION,
	['Gemini'] = TYPE_DUAL,
	['Tuner'] = TYPE_TUNER,
	['Synchro'] = TYPE_SYNCHRO,
	['Flip'] = TYPE_FLIP,
	['Toon'] = TYPE_TOON,
	['Xyz'] = TYPE_XYZ,
	['Pendulum'] = TYPE_PENDULUM,
	['Link'] = TYPE_LINK
}
attributes = {
	['earth'] = ATTRIBUTE_EARTH,
	['water'] = ATTRIBUTE_WATER,
	['fire'] = ATTRIBUTE_FIRE,
	['wind'] = ATTRIBUTE_WIND,
	['light'] = ATTRIBUTE_LIGHT,
	['dark'] = ATTRIBUTE_DARK,
	['divine'] = ATTRIBUTE_DEVINE,
	['EARTH'] = ATTRIBUTE_EARTH,
	['WATER'] = ATTRIBUTE_WATER,
	['FIRE'] = ATTRIBUTE_FIRE,
	['WIND'] = ATTRIBUTE_WIND,
	['LIGHT'] = ATTRIBUTE_LIGHT,
	['DARK'] = ATTRIBUTE_DARK,
	['DIVINE'] = ATTRIBUTE_DEVINE
}

function treatedAsPattern(archName)
	return "%(This card is always treated as an? \""..archName.."\" card%.%)"
end
function notTreatedAsPattern(archName)
	return "%(This card is not treated as an? \""..archName.."\" card%.%)"
end
summonAsPatternDesc = "as an? %a+ Monster %(([%a-]+)/(%u+)/Level (%d+)/ATK (%d+)/DEF (%d+)%)" --Monster stats in description
summonAsPatternNotes = "%[%[SummonAs:%(([%a-]+)/(%u+)/Level (%d+)/ATK (%d+)/DEF (%d+)%)%]%]" --Monster stats in notes (in case description doesn't follow the format for some reason)