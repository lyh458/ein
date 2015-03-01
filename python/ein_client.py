#!/usr/bin/env python
import rospy
import std_msgs
import roslib
#roslib.load_manifest("baxter_pick_and_place")
import readline

readline.parse_and_bind('tab: complete')
readline.parse_and_bind('set editing-mode emacs')



class SimpleCompleter(object):
    
    def __init__(self, options):
        self.options = sorted(options)
        return

    def complete(self, text, state):
        response = None
        if state == 0:
            # This is the first time for this text, so build a match list.
            if text:
                self.matches = [s 
                                for s in self.options
                                if s and s.startswith(text)]
            else:
                self.matches = self.options[:]
        
        # Return the state'th item from the match list,
        # if we have that many.
        try:
            response = self.matches[state]
        except IndexError:
            response = None
        return response


class EinClient:
    def __init__(self, words):
        self.forth_command_publisher = rospy.Publisher("/ein/left/forth_commands", 
                                                       std_msgs.msg.String, queue_size=10)
        
        readline.set_completer(SimpleCompleter(words).complete)

    def ask(self):

        while True:
            line = raw_input('Prompt ("stop" to quit): ')
            if line == 'stop':
                break
            print 'ENTERED: "%s"' % line
            self.forth_command_publisher.publish(line);

def main():
    rospy.init_node("ein_client")
    words = []
    for wordline in open("wordFile.ein"):
        words.append(wordline.split(" ")[0])
        
    print words

    client = EinClient(words)

    client.ask()


    
if __name__=='__main__':
    main()