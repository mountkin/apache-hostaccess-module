# Apache hostaccess module

This apache module implements an AllowedDomains directive for apache, which 
helps one to configure the apache server to only accept requests whose 
Host header is in the whitelist. Requests whose Host header are not in 
the whitelist will be denied by closing the TCP connection directly.

## Usage Scenarios

When you have more than one ServerName or ServerAlias in the same 
VirtualNameHost, but you want some of the directories only can be accessed
by some granted domain name.

## Runtime Configuration

You can put the following either in your .htaccess file or in the apache main 
config files.
```
AllowedDomains *.example.com other.test.com
```
