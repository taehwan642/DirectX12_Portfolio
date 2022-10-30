-- a = 30

transform = {}
transform["x"] = 0
transform["y"] = 0

function Update(elapsedTime)
	print("elapsedTime : ("..elapsedTime..")")
	transform["x"] = transform["x"] + 10
	transform["y"] = transform["y"] + 5
	return transform
end