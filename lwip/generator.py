import os

wsock = file(r"c:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Include\WinSock2.h")
code = open(os.getcwd()+"\generator.src")
i = 0
j = 0
k = 0
count = 0
strs = ""
lines = wsock.readlines(100000)
if lines:
	while 1:
		line1 = code.readline()
		if not line1:
			break
		line2 = line1.replace('\r','').replace('\n','')+'('
		for i in range(0,len(lines)):
			if lines[i].find(line2) == 0 :
				count += 1
				#print '%d' %i + " ----" + lines[i]
				while lines[i-j].find("WINSOCK_API_LINKAGE") < 0 and j < 10:
					j += 1
				while lines[i+k].find(";") < 0 and k < 30:
					k += 1
				#print '%d' %(i-j) + " ---------------------------->" + '%d' %(i+k) + " func:"+line1
				print "//---"+'%d'%count+"---------------------------------------------------------------"
				for l in range(i-j+1, i+k+1):
					strs += lines[l].replace('\r',' ').replace('\n',' ');
				strs = strs.replace('    __in', '');
				strs = strs.replace('    __out', '');
				strs = strs.replace(' _opt', '');
				strs = strs.replace('   ', '');
				strs = strs.replace(' out_opt', '');
				strs = strs.replace('       ', '');
				strs = strs.replace(';', '');
				strs = strs.replace('  ', ' ');
				strs = strs.replace('  __deref_inout_opt', '');
				strs += '\r\n{\r\n}'
				strs = 'extern "C" '+strs
				print strs
				j = 0
				k = 0
				strs = ""
				break
		print "//---"+line1.replace('\r','').replace('\n','')+"---------------------------------------------------------------"