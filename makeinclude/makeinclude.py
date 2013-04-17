import os.path

list = []

def Parse(target="XUI_include.h"):
	n = 0
	for n in range(len(list)):
		if list[n] == target:
			return "\n"
	list.append(target)
	
	if os.path.exists(target) == True:
		f = open(target,"r")
		lines = f.readlines();
		f.close()
		print("Read file : " +target + " success\n")
		i = 0
		file = ""
		temp = ""
		for i in range(len(lines)):
			line = lines[i]
			#include current directory file will be parse
			if "#include \"" in line and not "\\" in line and not "/" in line:
				file = line[10:-2]
				if os.path.exists(file) == False:
					print("Can't find "+ file +"\n")
					continue
				lines[i] = Parse(file)
			#delete '#pragma once'
			if "#pragma once" in line:
				lines[i] = "\n"
			temp += lines[i]
		return "//////////////////////////\n////"+ target + "\n//////////////////////////\n" + temp +"\n//////////////////////////\n////End " + target + "\n//////////////////////////\n"
	else:
		print("Can't find "+target+"\n")
		return ""

if __name__ == "__main__":
	i = Parse()
	if i != "":
		f = open("XUI_include_new.h", "w")
		f.write(i)
		f.close()
		print("Sava as XUI_include_new.h Complete!")
	else:
		print("Failed!")
	
