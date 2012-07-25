
function receive (prod)
	local status, value = coroutine.resume(prod)
	return value
end

function send (x)
	coroutine.yield(x)
end

function f_producer ()
	print("enter producer.")
	while true do
		local x = io.read()
		send(x)
	end
end

function filter (prod)
	return function ()
		for line = 1, math.huge do
			local x = receive(prod)
			x = string.format("%5d %s", line, x)
			send(x)
		end
	end
end

function consumer (prod)
	while true do
		local x = receive(prod)
		io.write(x, "\n")
	end
end

producer = coroutine.create(f_producer)
f = coroutine.create(filter(producer))

consumer(f)
