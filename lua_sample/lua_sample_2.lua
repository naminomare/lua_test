--�C���N�������g�������̂��h�����
--�f�N�������g���R�����g�Ɏg���Ă��܂��Ă��邹�����낤�����
function func_co()
	local call_count = 0
	while true do
		call_count = call_count + 1
		coroutine.yield(call_count);
	end
end

--local co = coroutine.wrap(func_co)
--print( co() )
--print( co() )
--print( co() )