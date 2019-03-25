import simpy
import numpy as np
import math



class Job:
    def __init__(self, name, arrtime, duration):
        self.name = name
        self.arrtime = arrtime
        self.firstarrtime = arrtime
        self.duration = duration
        self.responsetime = -1

class Server:
    def __init__(self, env, njobs):
        self.Jobs = list(())
        self.serversleeping = None

        self.terminal = Terminal(env, self)
        ''' statistics '''
        self.waitingTime = 0
        self.idleTime = 0
        self.quantum = 0.2

        '''CPU utilization calculation'''
        self.begintime = -1
        self.endtime = 0
        self.utilization = 0
        self.processingtime = 0

        ''' response time '''
        self.averesptime = 0
        self.maxresptime = 0


        self.NJob = njobs
        self.NJobDone = 0
        ''' register a new server process '''
        self.servproc = env.process( self.serve(env) )

    def serve(self, env):
        while True:
            ''' do nothing, just change server to idle
            and then yield a wait event which takes infinite time
            '''
            if (self.begintime == -1):
                self.begintime = env.now



            if len( self.Jobs ) == 0 :      # if joblist is empty
                self.serversleeping = env.process( self.waiting( env ))
                t1 = env.now
                yield self.serversleeping
                ''' accumulate the server idle time'''
                self.idleTime += env.now - t1
            else:
                ''' get the first job to be served'''
                j = self.Jobs.pop( 0 )
                print("Job name: %s, arrTime %5.5f and remaining duration %5.5f" % (j.name, j.arrtime, j.duration))
                ''' sum up the waiting time'''
                self.waitingTime += env.now - j.arrtime
                j.arrtime = env.now
                ''' f '''
                if (j.responsetime == -1):
                    j.responsetime = env.now
                    tmp = j.responsetime - j.firstarrtime
                    self.averesptime += tmp
                    if (self.maxresptime < tmp):
                        self.maxresptime = tmp

                ''' yield an event for the job finish'''
                if (j.duration > self.quantum):
                    yield env.timeout( self.quantum )

                    j.arrtime = env.now
                    j.duration -= self.quantum
                    self.processingtime +=  self.quantum
                    self.Jobs.append(j)
                        
                else:
                    yield env.timeout( j.duration )
                    self.processingtime +=  j.duration
                    self.NJobDone += 1
                    if (self.NJobDone == self.NJob ):
                        ''' Reach threshold '''
                        self.endtime = env.now
                        print('End at %.8f' % self.endtime)

                        self.terminal.done()
                        self.averesptime /= self.terminal.NJobGen
                        return
                    

    def waiting(self, env):
        try:
            print( 'Server is idle at %5.5f' % env.now )
            yield env.timeout(1000)
        except simpy.Interrupt as i:
            print('A new job comes. Server waken up and works now at %5.5f' % env.now )

    def done(self):
        self.servproc.interrupt()

class Terminal:
    def __init__(self, env, server):
        self.server = server
        self.NJobGen = 0
        self.terminal = env.process( self.generatejobs(env) )

    def generatejobs(self, env):
        i = 1
        while True:
            try:
                '''yield an event for new job arrival'''
                job_interarrival = self.thinking_time(np.random.rand())
                yield env.timeout( job_interarrival )                               # event: job's arrival

                ''' generate service time and add job to the list'''
                job_duration = self.serving_time(np.random.rand())
                self.server.Jobs.append( Job('Job %s' %i, env.now, job_duration) )
                print( 'job %d: arrive at = %5.5f, length = %5.5f' %( i, env.now, job_duration ) )
                i += 1
                self.NJobGen += 1

                ''' if server is idle, wake it up'''
                if not self.server.serversleeping.triggered:
                    self.server.serversleeping.interrupt( 'Wake up, please.' )
            except:
                return

    def thinking_time(self, x, beta = 1/15):
        return beta * math.exp(-x * beta)

    def serving_time(self, x, beta = 1/0.5):
        return beta * math.exp(-x * beta)

    def done(self):
        self.terminal.interrupt()

env = simpy.Environment()
# this example has 2 processes: MyServer and MyTerminal
MyServer = Server( env, 10)

env.run()
print( '****** STATISTIC ******')
print( 'Total waiting time     : %.4f' % MyServer.waitingTime )
print( 'Mean waiting time      : %.4f' % (MyServer.waitingTime/1000) )
print( 'Total server idle time : %.4f' % MyServer.idleTime )

print( 'processing time = %.8f' % MyServer.processingtime)
print( 'end time = %.8f' % MyServer.endtime)
print( 'begin time = %.8f' % MyServer.begintime)
print( 'CPU utilization        : %.8f %.8f' % (MyServer.processingtime, (MyServer.endtime - MyServer.begintime - MyServer.idleTime)))

print( 'Response Time Section: ')
print( 'Average response time  : %.6f' % MyServer.averesptime)
print( 'Max response time      : %.6f' % MyServer.maxresptime)
