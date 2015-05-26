import pexpect

def ssh_cmd(ip, passwd, cmd):
    ret = -1
    ssh = pexpect.spawn('ssh yliu@%s "%s"' % (ip, cmd))
    try:
        i = ssh.expect(['.*password:', 'continue connecting (yes/no)?'], timeout                                                                                             =5)
        if i == 0 :
            ssh.sendline(passwd)
        elif i == 1:
            ssh.sendline('yes\n')
            ssh.expect('password: ')
            ssh.sendline(passwd)
        ssh.sendline(cmd)
        r = ssh.read()
        print r
        ret = 0
    except pexpect.EOF:
        print "EOF"
        ssh.close()
        ret = -1
    except pexpect.TIMEOUT:
        print "TIMEOUT"
        ssh.close()
        ret = -2
    return ret
def copy(remote, passwd):
    ret = -1
    ssh = pexpect.spawn('scp /auto/sharebook_server/BookWebServer/target/BookWeb                                                                                             Server-1.2.3.RELEASE.jar yliu@%s:/wind/sharebook/web/BookWebServer-1.0.jar' %(re                                                                                             mote))
    try:
        i = ssh.expect(['.*password:', 'continue connecting (yes/no)?'], timeout                                                                                             =5)
        if i == 0 :
            ssh.sendline(passwd)
        elif i == 1:
            ssh.sendline('yes\n')
            ssh.expect('.*password: ')
            ssh.sendline(passwd)
        r = ssh.read()
        print r
        ret = 0
    except pexpect.EOF:
        print "copy finished"
        ssh.close()
        ret = -1
    except pexpect.TIMEOUT:
        print "TIMEOUT"
        ssh.close()
        ret = -2
    return ret
def build():
    git = pexpect.spawn('mvn clean package')
    try:
        r = git.read()
        print r
    except pexpect.EOF:
        print "build finished"
        git.close()
    except pexpect.TIMEOUT:
        git.close()
def update(user, pwd):
    git = pexpect.spawn('git pull')
    try:
        i = git.expect('Username.*')
        if i == 0 :
            git.sendline(user)
        i = git.expect('Password.*')
        if i == 0 :
            git.sendline(pwd)
        r = git.read()
        print r
    except pexpect.EOF:
        print "update finished"
        git.close()
    except pexpect.TIMEOUT:
        git.close()
print "---------------git pull---------------"
i = update("jluo","1234qwer")
print i
print "---------------build------------------"
i = build()
print i
print "---------------deploy-----------------"
i = copy("10.101.1.165","Qinmen2016!")
print i
print "---------------restart----------------"
i = ssh_cmd("10.101.1.165","Qinmen2016!","ps -ef | grep BookWebServer-1.0.jar |                                                                                              grep -v grep | awk '{print $2}' | xargs kill -9 2>&1;echo 'kill '$?")
print i
i = ssh_cmd("10.101.1.165","Qinmen2016!","~/run.sh")
print i
