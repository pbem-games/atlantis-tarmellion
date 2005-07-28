#!/bin/bash
#script to run the real turn

COUNT=$(grep "TurnNumber:" players.in | cut -f2 -d" ")
COUNT=`expr $COUNT + 1`

echo This is turn $OPTION run number $COUNT
sleep 3 

echo Getting orders

cp web-advance/$COUNT/orders.* .
cp email/orders.* .
cp web/orders.* .
    
if [ $? -eq 0 ]; then
	
    echo Determining missed turns
    cp players.in players.in.before-missed-turns
    script/missing-orders.pl $COUNT

    echo Determining order rewards
    cp players.in players.in.before-times-rewards
    script/order-rewards.pl $COUNT real

    if [ $? -eq 0 ]; then
	echo adding new signups
	cat newplayers.in >> players.in

	echo Rating times posts
	cp players.in players.in.before-post-ratings
	script/rate_posts.pl 

	echo Determining times post rewards
	script/post-rewards.pl real

	echo Running turn
	./tarmellion run

	if [ $? -eq 0 ]; then
	    echo Filing turn at turn$COUNT
	    mkdir turn$COUNT
    
	    cp report.* turn$COUNT/
	    mv orders.* turn$COUNT/
	    cp *.out turn$COUNT/
	    cp game.in turn$COUNT/
	    cp players.in turn$COUNT/players.in.after_scripts
	    cp players.in.before-missed-turns turn$COUNT/players.in
	    cp newplayers.in turn$COUNT/
		
	    if [ $? -eq 0 ]; then
		echo Making ready for next game
	    
		mv game.out game.in
		mv players.out players.in
		chgrp www-data players.in
		chmod 664 players.in

		if [ $? -eq 0 ]; then

#		    echo Checking number of heros
#		    ./script/herotester.pl 

		    echo Checkinh for Battles for villages
		    ./script/villagetester.pl

		    echo Zipping reports
		    ./script/zipping.pl

		    echo Sending out mails
		    ./script/email-out.pl $COUNT

		    echo Creating new default orders
		    ./script/make_orders.pl

		    echo Filing reports for download
		    rm -f /home/tarmellion/html/report/report.*
		    ./script/report2download.pl
		    rm -f report.*

		    echo Cleaning input directories
		    rm -fr web-advance/$COUNT
		    rm -f email/orders.*
		    rm -f web/orders.*
		    cp -p aa newplayers.in
		    cp orders.3 web/
		    sleep 2
		    cp orders.4 web/
		    sleep 2
		    cp orders.5 web/
		    sleep 2
		    cp orders.6 web/
		    sleep 2
		    cp orders.7 web/
		    sleep 2
		    cp orders.8 web/
		    sleep 2
		    cp orders.9 web/
		    sleep 2
		    cp orders.10 web/
		    sleep 2
		    cp orders.11 web/
		    sleep 2
		    cp orders.12 web/
		    sleep 2
		    cp orders.13 web/
		    sleep 2
		    cp orders.14 web/
		    chmod 664 web/orders.*
		    
		    echo Doing weekly backup
		    ./script/create-backup.sh weekly
		fi
	    fi
	fi
    fi
fi

echo Everything done





