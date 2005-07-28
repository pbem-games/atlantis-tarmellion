#!/bin/bash
#script to run a testrun

COUNT=$(grep "TurnNumber:" players.in | cut -f2 -d" ")
COUNT=`expr $COUNT + 1`
FLAG=$1

echo This is a test turn run for turn $COUNT

echo Getting orders

cp web-advance/$COUNT/orders.* .
cp email/orders.* .
cp web/orders.* .

cp -p players.in players.in.save

if [  $? -eq 0 ]; then

   if [ "$FLAG" = "short" ]; then
   	echo No rewards computed
   else
	echo Determining missed turns
	cp players.in players.in.before-missed-turns

	script/missing-orders.pl $COUNT

        echo Determining order rewards
	cp players.in players.in.before-times-rewards

	script/order-rewards.pl $COUNT real

   fi

	if [  $? -eq 0 ]; then

	    echo adding new signups

	    cat newplayers.in >> players.in

	    if [ "$FLAG" = "short" ]; then
		   echo No rewards computed
	    else

		   echo Rating Times posts

		   cp players.in players.in.before-post-ratings

		   script/rate_posts.pl test

		   echo Determining times post rewards
		
		   script/post-rewards.pl real

	    fi

	    echo Running turn
	
	    ./tarmellion run

	    if [  $? -eq 0 ]; then
		if [ "$FLAG" = "short" ]; then
		    echo No check on hero status or villages
		else
		    echo Checking number of heros
		    ./script/herotester.pl

		    echo Checking battles for villages
		    ./script/villagetester.pl test
		fi	
		echo Filing turn at testturn

		rm -f testturn/*
    
		mv report.* testturn/
		cp orders.* testturn/
		mv *.out testturn/
		cp *.in testturn/

		echo Checking NPC reports
		./script/npc-checker.pl

	     fi
	fi
fi

cp -p players.in.save players.in
	

echo Everything done




