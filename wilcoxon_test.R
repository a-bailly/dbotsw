# CMD: Rscript wilcoxon_test.R

library(coin)
data = read.csv("ucr_error_rates.csv", head = TRUE, sep = ',')
data[data < 0] = NA

colnames = names(data)

for (v in c("BoP", "BOSS", "COTE", "DTWNN", "EDNN", "PROP", "SAXVSM", "SMTS", "TSBF")) {
	indices = !is.na(data[, v])
	print(sprintf("D-BoTSW better than %s : %.3f", v, pvalue(wilcoxsign_test(data[indices, "D.BoTSW"]~data[indices, v], alternative="less", distribution="exact"))))
}

for (v in c("BOSS", "COTE", "PROP")) {
	indices = !is.na(data[, v])
	print(sprintf("%s better than D-BoTSW : %.3f", v, pvalue(wilcoxsign_test(data[indices, v]~data[indices, "D.BoTSW"], alternative="less", distribution="exact"))))
}

