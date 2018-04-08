
x <- build("1 2 4 8 16 32"," ")
y1 <- sapply(build("0.040063903 0.008235911 0.120975517 0.355172566 0.355671263 0.355771263"," "),as.numeric)
s <- spline(x, y1,method="natural")
plot(y1 ~ x,ylim=c(0,2.10), xlimit=c(1,16), log="x",xaxt="n", ylab="Speedup (x)",main="Speedup of fibonacci with tasks (1st approach)")
axis(side=1,at=c(1,2,4,8,16,32))
mtext(side=1,text="Nº Threads",line=1.5)
abline(h=1, type="l", lty=2)
lines(s)
legend(1, 1.10, legend=c(build("fibonacci.c")), cex=0.8, bty="n")
legend(1, 0.14, legend=c(build("fibonacci-p1.c")), cex=0.8, bty="n")

