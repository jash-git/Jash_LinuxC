# this is a simple shutdown
# and will be saved as shutdown.sh in the system
echo WowzaDisconnectAnalytics is shutting down
echo done shutting down WowzaDisconnectAnalytics
echo "asd700502" |sudo -S kill -9 `ps aux | grep WowzaDisconnectAnalytics | awk '{print $2}'`
