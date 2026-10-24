using Characters.States;
using UnityEngine;
using UnityEngine.InputSystem;

namespace Characters.Movement
{
    public class CharacterController : MonoBehaviour
    {
        [SerializeField] private InputActionReference moveAction;

        [SerializeField] private float movementSpeed = 2.7f;

        [SerializeField] private float jumpSpeed = 5f;

        [SerializeField] private LayerMask groundLayer;

        private Collider characterCollider;

        private Rigidbody rigidBody;

        private Vector2 velocity;

        private Vector2 movementDirection;

        private MovementStates movementState;

        private bool stickIsUp;

        private bool stickWasUp;

        private bool jumpRequested;

        private int remainingJumps;

        private void Awake()
        {
            characterCollider = GetComponent<Collider>();
            rigidBody = GetComponent<Rigidbody>();
        }

        void Start()
        {
            moveAction.action.Enable();

            movementState = MovementStates.STANDING;
            stickIsUp = false;
            stickWasUp = false;
        }

        void Update()
        {
            // Updates movement direction
            movementDirection = moveAction.action.ReadValue<Vector2>();
            movementDirection.x = Mathf.Round(movementDirection.x);
            movementDirection.y = Mathf.Round(movementDirection.y);

            // Checks if a jump was requested
            stickIsUp = movementDirection.y > 0;
            if (stickIsUp && !stickWasUp) jumpRequested = true;
            stickWasUp = stickIsUp;
        }

        private void FixedUpdate()
        {
            velocity = rigidBody.linearVelocity;

            if (IsGrounded())
            {
                // Moves horizontally
                velocity.x = -movementDirection.x * movementSpeed;  // inverted for some reason
                SetGrounded();
            }

            if (jumpRequested && remainingJumps > 0)
            {
                Jump();
            }

            // Updates position
            rigidBody.linearVelocity = velocity;
        }

        private bool IsGrounded()
        {
            // Distance checked for colliders below the character
            const float groundCheckDistance = 0.05f;

            Bounds characterBounds = characterCollider.bounds;
            // Gets a point just below the character's feet
            Vector3 origin = new Vector3(characterBounds.center.x, characterBounds.min.y + 0.01f, characterBounds.center.z);
            return Physics.Raycast(origin, Vector3.down, groundCheckDistance + 0.01f, groundLayer);
        }

        private void Jump()
        {
            velocity.x = -movementDirection.x * movementSpeed;  // necessairy for being able to change directions when jumping
            velocity.y = jumpSpeed;

            movementState = MovementStates.JUMPING;
            jumpRequested = false;
            remainingJumps--;
        }

        private void SetGrounded()
        {
            movementState = MovementStates.STANDING;
            remainingJumps = 2;
        }
    }
}
