# Cerver
This is an easy-to-use,static web-server written entirely in C. Curently you can serve static HTML, CSS, and JavaScript files just by creating a handler struct and passing it the API route, file path, document type, and the serve_static builder function.
<br>
<br>

## Motivation
I want to replicate the simplicity and ease-of-use that other backend frameworks like Express.js use to create web servers entirely in C!
<br>
<br>

## Installing
To build the library file to use in a given project run:
```
make library
```
This currently moves the .so and header file to a lib directory that sits outside the current file.
<br>
<br>
<br>
There is a test server you can run with the index.html file provided. The commands to compile and run the server are:
```
make test
./a.out
```

### Side note
I broke this off from the c-what-you-can-do repository. If you would like to see the full commit history I kept that repository up to check it out.
