#!/bin/bash
S1='Timestamp WrongValue ThreadTimestamp ThreadID EventType EventSubject Other'

if [ -r profiling.log ]; 
then {
	sed -e 's/schedule: /schedule:/' profiling.log >profiling1.log
	echo "First step done.."
	sed -e 's/duration: /duration:/' profiling1.log >profiling2.log
	echo "Second step done.."
	rm -r profiling.log
	echo "Third step done.."
	rm -r profiling1.log
	echo "Fourth step done.."
	mv profiling2.log profiling.log
	echo "Fifth step done.."
	
		if grep -Fxq "$S1" profiling.log
			then
    			echo "Skip step six line already existent!!!"
			else
    			echo "Timestamp WrongValue ThreadTimestamp ThreadID EventType EventSubject Other"|cat - profiling.log > /tmp/out && mv /tmp/out profiling.log
				echo "Sixth step done.."
	fi
	
	echo "Done script."
	echo "Thanks for using me. Made by Gabe"
}
else 
echo "FATTAL ERROR:: The file profiling.log is not in the same location as the script."
fi