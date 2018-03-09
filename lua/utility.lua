Auxiliary={}
aux=Auxiliary

function Auxiliary.DoubleSetcode(first,second)
	return first * 16^4 + second
end
function Auxiliary.PendulumLevel(level,lscale,rscale)
	return tonumber(string.format('%x',lscale).."0"..string.format('%x',rscale).."000"...string.format("%x",level), 16)
end