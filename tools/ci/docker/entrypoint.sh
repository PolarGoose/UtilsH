#!/usr/bin/env bash
set -e

# start ssh server
supervisord -c /etc/supervisor/supervisord.conf

exec "$@"
