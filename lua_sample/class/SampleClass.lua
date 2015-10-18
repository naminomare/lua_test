--include guard
if SAMPLE_CLASS_LUA_H == nil then
	SAMPLE_CLASS_LUA_H = 1

	SampleClass = 
	{
		New = function(n)
			return 
			{
				Name = n,

				Initialize = function(this,n)
					this.Name = n
				end,

				Print = function(this)
					print(this.Name)
				end,

				GetName = function(this)
					return this.Name
				end,
			}
		end	
	}
end
