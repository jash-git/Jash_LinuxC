# this is a simple shutdown
# and will be saved as shutdown.sh in the system
echo cpp_service is shutting down
echo done shutting down cpp_service
echo "asd700502" |sudo -S kill -9 `ps aux | grep cpp_service | awk '{print $2}'`
