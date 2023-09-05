echo "c run test start"
echo "c install ..."
bash ./install.sh

echo "c compress ..."
bash ./compress.sh ./data/res_b20.txt ./tmp/res_b20_compressed

echo "c uncompress ..."
bash ./uncompress.sh ./tmp/res_b20_compressed ./tmp/res_b20_uncompressed.txt

echo "c diff original data and uncompressed data ..."
diff ./data/res_b20.txt ./tmp/res_b20_uncompressed.txt

echo "c END"
