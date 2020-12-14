#========================================================
# R statistics system grapher for Comp111 Assignment 4
#========================================================

#========================================================
# read whole history from file 'output.csv' 
#========================================================
allHistory<- function(x, y, z) UseMethod('allHistory')

allHistory.NULL <- function(x) { allHistory('output.csv') } 

### cstate <- function(stuff) { 
###     return (stuff == 'idle' ? 0 : 
### 		(stuff == 'slurping' ? 1 : 
### 		    (stuff == 'swallowing' ? 2 : 
### 			(stuff == 'sulking' ? 3 : NA)))); 
### } 

# read results from file 
allHistory.character <- function(file) { 
  out <- read.csv(file, header=FALSE)
  out <- data.frame(when=out$V1, who=out$V2, what=out$V3, where=out$V4)
  class(out) <- c('allHistory', class(out))
  out 
} 

# scope results in time 
allHistory.allHistory <- function(all, low=0, high=1e25) { 
  sel <- (all$when >= low) & (all$when <= high)
  out <- data.frame(when=all$when[sel], who=all$who[sel], 
	what=all$what[sel], where=all$where[sel])
  class(out) <- c('allHistory', class(out))
  out
}

#========================================================
# smart plotting routines describe how to plot 
# based upon object class. 
#========================================================

# plot all job histories.
plot.allHistory <- function(all, low=0, high=1e25) { 
    toplot <- allHistory(all, low, high) 
    # print(toplot)
    # create a selected region from this! 
    plot(c(), type='l', xlab='time', ylab='aardvark', 
	  xlim=c(0,toplot$when[[length(toplot$when)]]), ylim=c(0,10))
    lastwhat  <- list(); 
    lastwhen  <- list(); 
    lastwhere <- list(); 
    ypos      <- list(); 

    # aardvarks and anthills
    aardvarks = c('A','B','C','D','E','F','G','H','I','J','K')
    anthills = c(1,2,3)
### states = c('idle','slurping','swallowing','sulking','jailed')
### # colors for states 
### scolors <- list(); 
### scolors[['idle']]       <- 'gray' 
### scolors[['swallowing']] <- 'yellow' 
### scolors[['sulking']]    <- 'red' 
### scolors[['jailed']]    <- 'orange' 

    # colors for hills 
    hcolors <- list() 
    hcolors[[1]] <- 'darkgreen' 
    hcolors[[2]] <- 'green' 
    hcolors[[3]] <- 'lightgreen' 

    # data on each anteater 
    count <- 0; 
    for (who in aardvarks) { 
	lastwhat[[who]]='idle' 
	lastwhen[[who]]=0 
	lastwhere[[who]]=NA
	ypos[[who]]=count 
 	count <- count+1
    } 
 
    # rectangle arrays for rendering 
    rectangles = list(); 
    for (depict in c('idle','swallowing','sulking','jailed')) { 
	rectangles[[depict]]=list(); 
	for (dimension in c('xbot','xtop', 'ybot', 'ytop')) { 
	    rectangles[[depict]][[dimension]]=array() 
	} 
    } 
    srects = list(); 
    for (where in anthills) { 
	srects[[where]]=list() 
	for (dimension in c('xbot','xtop', 'ybot', 'ytop')) { 
	    srects[[where]][[dimension]]=array() 
	} 
    } 

    # record state for each point 
    for (index in c(1:length(toplot$when))) { 
	who      <- as.character(toplot$who[[index]])  # unfactor
	newwhat  <- as.character(toplot$what[[index]]) # unfactor 
	newwhen  <- toplot$when[[index]] 
	newwhere <- toplot$where[[index]] 
	oldwhat  <- lastwhat[[who]]
	oldwhen  <- lastwhen[[who]]
	oldwhere <- lastwhere[[who]]
        # print(paste("index=",index))
        # print(paste('who=',who)) 
        # print(paste('oldwhat=',oldwhat)) 
        # print(paste('oldwhen=',oldwhen)) 
        # print(paste('oldwhere=',oldwhere)) 
        # print(paste('newwhat=',newwhat)) 
        # print(paste('newwhen=',newwhen)) 
        # print(paste('newwhere=',newwhere)) 
	if (oldwhat!='slurping') { 
	    count <- length(rectangles[[oldwhat]][['xbot']])+1
	    rectangles[[oldwhat]][['xbot']][[count]] <- oldwhen; 
	    rectangles[[oldwhat]][['ybot']][[count]] <- ypos[[who]]-0.25
	    rectangles[[oldwhat]][['xtop']][[count]] <- newwhen; 
	    rectangles[[oldwhat]][['ytop']][[count]] <- ypos[[who]]+0.25
	} else { 
  	    count <- length(srects[[oldwhere+1]][['xbot']])+1
  	    srects[[oldwhere+1]][['xbot']][[count]] <- oldwhen; 
  	    srects[[oldwhere+1]][['ybot']][[count]] <- ypos[[who]]-0.25
  	    srects[[oldwhere+1]][['xtop']][[count]] <- newwhen; 
  	    srects[[oldwhere+1]][['ytop']][[count]] <- ypos[[who]]+0.25
	} 
	lastwhen[[who]] = newwhen
	lastwhat[[who]] = newwhat 
	lastwhere[[who]] = newwhere 
    } 
    # now clean up: display final rectangles 
    # compute latest time in trace 
    newwhen = NA
    for (who in aardvarks) { 
	if (is.na(newwhen) ||  newwhen<lastwhen[[who]]) { 
	    newwhen = lastwhen[[who]]; 
	} 
    } 
    for (who in aardvarks) { 
	oldwhen  <- lastwhen[[who]]
	oldwhat  <- lastwhat[[who]]
	oldwhere <- lastwhere[[who]]
	if (oldwhat != 'slurping') { 
	    count <- length(rectangles[[oldwhat]][['xbot']])+1
	    rectangles[[oldwhat]][['xbot']][[count]] <- oldwhen; 
	    rectangles[[oldwhat]][['ybot']][[count]] <- ypos[[who]]-0.25
	    rectangles[[oldwhat]][['xtop']][[count]] <- newwhen; 
	    rectangles[[oldwhat]][['ytop']][[count]] <- ypos[[who]]+0.25
	} else { # add concept of "where"
  	    count <- length(srects[[oldwhere+1]][['xbot']])+1
  	    srects[[oldwhere+1]][['xbot']][[count]] <- oldwhen; 
  	    srects[[oldwhere+1]][['ybot']][[count]] <- ypos[[who]]-0.25
  	    srects[[oldwhere+1]][['xtop']][[count]] <- newwhen; 
  	    srects[[oldwhere+1]][['ytop']][[count]] <- ypos[[who]]+0.25
	} 
    } 

    for (depict in c('slurping', 'idle','swallowing','sulking', 'jailed')) {
	if (depict == 'slurping') { 
 	    for (where in anthills) { 
 		color <- hcolors[[where]]
 		plotlist <- srects[[where]]
 		rect(plotlist$xbot,plotlist$ybot,plotlist$xtop,plotlist$ytop,col=color,border=color)
 	    } 
	} else if (depict=='idle') { 
	    color <- 'gray'
	} else if (depict=='sulking') { 
	    color <- 'red'
	} else if (depict=='swallowing') { 
	    color <- 'yellow'
	} else if (depict=='jailed') { 
	    color <- 'orange'
	} else { 
	    color <- 'purple'
	} 
	plotlist <- rectangles[[depict]]
	rect(plotlist$xbot,plotlist$ybot,plotlist$xtop,plotlist$ytop,col=color,border=color)
    }
} 

#========================================================
# use the system to visualize results
#========================================================

# read file from disk into full history 
all <- allHistory()
plot(all); 
