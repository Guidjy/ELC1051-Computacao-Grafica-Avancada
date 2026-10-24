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

        private const float groundCheckDistance = 0.05f;  // Distance checked for colliders below the character

        private void Awake()
        {
            characterCollider = GetComponent<Collider>();
            rigidBody = GetComponent<Rigidbody>();
        }

        void Start()
        {
            moveAction.action.Enable();

            movementState = MovementStates.STANDING;
        }

        void Update()
        {
            movementDirection = moveAction.action.ReadValue<Vector2>();
        }

        private void FixedUpdate()
        {
            velocity = rigidBody.linearVelocity;

            // Moves horizontally
            if (IsGrounded())
            {
                velocity.x = -movementDirection.x * movementSpeed;  // inverted for some reason
            }
            else
            {
                velocity.x = velocity.x;
            }

            // Jumps if grounded
            if (IsGrounded() && movementDirection.y > 0)
            {
                velocity.y = jumpSpeed;
                movementState = MovementStates.JUMPING;
            }

            rigidBody.linearVelocity = velocity;
        }

        private bool IsGrounded()
        {
            Bounds characterBounds = characterCollider.bounds;
            // Gets a point just below the character's feet
            Vector3 origin = new Vector3(characterBounds.center.x, characterBounds.min.y + 0.01f, characterBounds.center.z);
            return Physics.Raycast(origin, Vector3.down, groundCheckDistance + 0.01f, groundLayer);
        }
    }
}
